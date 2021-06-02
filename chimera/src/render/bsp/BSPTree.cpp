#include "chimera/render/bsp/BSPTree.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

template <class T> void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

BspTree::BspTree() {
    root = nullptr;
    resultVertex = nullptr;
    logdata = false;
}

BspTree::~BspTree() { this->destroy(); }

void BspTree::create(bool leafy, std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex) {

    std::list<Triangle*> vTris;
    vVertex = _vVertex;

    if (_vIndex.size() > 0) {

        // Usa os indices já pre-calculado
        for (unsigned int indice = 0; indice < _vIndex.size(); indice += 3) {
            unsigned int pa = _vIndex[indice];
            unsigned int pb = _vIndex[indice + 1];
            unsigned int pc = _vIndex[indice + 2];

            // Calcula Normal Face
            glm::vec3 acc = vVertex[pa].normal + vVertex[pb].normal + vVertex[pc].normal;
            glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);

            // glm::vec3 u = vVertex[pb].position - vVertex[pa].position;
            // glm::vec3 v = vVertex[pc].position - vVertex[pa].position;
            // glm::vec3 normal2 = glm::normalize(glm::cross(u, v));

            vTris.push_back(new Triangle(pa, pb, pc, normal));
        }

    } else {

        // Calcula os indices na sequencia em que os vertices estão
        for (unsigned int indice = 0; indice < _vVertex.size(); indice += 3) {
            unsigned int pa = indice;
            unsigned int pb = indice + 1;
            unsigned int pc = indice + 2;

            // Calcula Normal Face
            glm::vec3 acc = vVertex[pa].normal + vVertex[pb].normal + vVertex[pc].normal;
            glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);

            // glm::vec3 u = vVertex[pb].position - vVertex[pa].position;
            // glm::vec3 v = vVertex[pc].position - vVertex[pa].position;
            // glm::vec3 normal2 = glm::normalize(glm::cross(u, v));

            vTris.push_back(new Triangle(pa, pb, pc, normal));
        }
    }

    if (leafy == false)
        root = build(vTris);
    else
        root = buildLeafy(vTris);
}

void BspTree::destroy() { collapse(root); }

void BspTree::collapse(BSPTreeNode* tree) {

    if (tree->front != nullptr) {
        collapse(tree->front);
        delete tree->front;
        tree->front = nullptr;
    }

    if (tree->back != nullptr) {
        collapse(tree->back);
        delete tree->back;
        tree->back = nullptr;
    }
}

void BspTree::drawPolygon(BSPTreeNode* tree, bool frontSide) {

    if (tree->pLeaf == nullptr)
        return;

    if (logdata == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Convex: %ld", tree->pLeaf->index.size());

    for (auto index : tree->pLeaf->index)
        resultVertex->push_back(vVertex[index]);
}

void BspTree::traverseTree(BSPTreeNode* tree, glm::vec3* pos) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    if (tree->isSolid == true)
        return;

    // no de indicador de final/solido
    // if (tree->pLeaf == nullptr)
    //     return;

    SIDE result = tree->hyperPlane.classifyPoint(pos);
    switch (result) {
        case SIDE::CP_FRONT:
            traverseTree(tree->back, pos);
            drawPolygon(tree, true);
            traverseTree(tree->front, pos);
            break;
        case SIDE::CP_BACK:
            traverseTree(tree->front, pos);
            drawPolygon(tree, false); // Elimina o render do back-face
            traverseTree(tree->back, pos);
            break;
        default: // SIDE::CP_ONPLANE
            // the eye point is on the partition hyperPlane...
            traverseTree(tree->front, pos);
            traverseTree(tree->back, pos);
            break;
    }
}

void BspTree::render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool _logData) {
    logdata = _logData;
    resultVertex = _pOutVertex;
    traverseTree(root, eye);
}

Triangle* BspTree::selectBestSplitter(std::list<Triangle*>& _vTriangle) {

    unsigned int bestScore = 100000;                // just set to a very high value to begin
    Triangle* selectedTriangle = nullptr;           // poit to none
    glm::vec3 temp;                                 // inutil
    long long score, splits, backfaces, frontfaces; // counts

    for (Triangle* th : _vTriangle) {

        if (th->beenUsedAsSplitter == true)
            continue;

        score = splits = backfaces = frontfaces = 0;

        Plane hyperPlane(th->position(vVertex, 0), th->getNormal());

        for (Triangle* currentPoly : _vTriangle) {
            if (currentPoly != th) {
                SIDE result = hyperPlane.classifyPoly(currentPoly->position(vVertex, 0), // PA
                                                      currentPoly->position(vVertex, 1), // PB
                                                      currentPoly->position(vVertex, 2), // PC
                                                      &temp);                            // Clip Test Result (A,B,C)
                switch (result) {
                    case SIDE::CP_ONPLANE:
                        break;
                    case SIDE::CP_FRONT:
                        frontfaces++;
                        break;
                    case SIDE::CP_BACK:
                        backfaces++;
                        break;
                    case SIDE::CP_SPANNING:
                        splits++;
                        break;
                    default:
                        break;
                }
            }
        } // end while current poly

        score = abs(frontfaces - backfaces) + (splits * 8);

        if (score < bestScore) {
            bestScore = score;
            selectedTriangle = th;
        }

    } // end while splitter

    selectedTriangle->beenUsedAsSplitter = true;
    return selectedTriangle;
}

void BspTree::splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle) {

    // Proporcao de textura (0.0 a 1.0)
    float propAC = 0.0;
    float propBC = 0.0;

    // ultima posicao de indice de vertices
    unsigned int last = vVertex.size();

    // Vertex dos triangulos a serem normalizados
    Chimera::VertexData vertA, vertB, vertC;

    // Pega pontos posicao original e inteseccao
    glm::vec3 A, B;
    glm::vec3 a = _pTriangle->position(vVertex, 0);
    glm::vec3 b = _pTriangle->position(vVertex, 1);
    glm::vec3 c = _pTriangle->position(vVertex, 2);

    // Normaliza Triangulo para que o corte do triangulo esteja nos segmentos de reta CA e CB (corte em a e b)
    if (fx.x * fx.z >= 0) {                     // corte em a e c (rotaciona pontos sentido horario) ABC => BCA
        swapFace(b, c);                         //   troca b com c
        swapFace(a, b);                         //   troca a com b
        vertA = _pTriangle->vertex(vVertex, 2); //   old c
        vertB = _pTriangle->vertex(vVertex, 0); //   old a
        vertC = _pTriangle->vertex(vVertex, 1); //   old b

    } else if (fx.y * fx.z >= 0) {              // corte em b e c (totaciona pontos sentido anti-horario)  ABC => CAB
        swapFace(a, c);                         //   troca A com C
        swapFace(a, b);                         //   torca a com b
        vertA = _pTriangle->vertex(vVertex, 1); //   old b
        vertB = _pTriangle->vertex(vVertex, 2); //   old c
        vertC = _pTriangle->vertex(vVertex, 0); //   old a

    } else {                                    // Cortre em a e b (pontos posicao original)
        vertA = _pTriangle->vertex(vVertex, 0); //   old a
        vertB = _pTriangle->vertex(vVertex, 1); //   old b
        vertC = _pTriangle->vertex(vVertex, 2); //   old c
    }

    hyperPlane.intersect(&a, &c, &A, &propAC);
    hyperPlane.intersect(&b, &c, &B, &propBC);

    // PA texture coord
    glm::vec2 deltaA = (vertC.texture - vertA.texture) * propAC;
    glm::vec2 texA = vertA.texture + deltaA;

    // PB texture coord
    glm::vec2 deltaB = (vertC.texture - vertB.texture) * propBC;
    glm::vec2 texB = vertB.texture + deltaB;

    // Calcula Normal Face
    glm::vec3 acc = vertA.normal + vertB.normal + vertC.normal;
    glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);

    //-- T1 Triangle T1(a, b, A);
    vVertex.push_back({a, vertA.normal, vertA.texture}); // T1 PA
    vVertex.push_back({b, vertB.normal, vertB.texture}); // T1 PB
    vVertex.push_back({A, vertA.normal, texA});          // T1 PC
    //_vTriangle.push_back(new Triangle(last++, last++, last++, normal));
    Triangle* th1 = new Triangle(last++, last++, last++, normal);
    th1->beenUsedAsSplitter = _pTriangle->beenUsedAsSplitter;
    _vTriangle.push_front(th1); // TODO :Testar se é isto mesmo

    // //-- T2 Triangle T2(b, B, A);
    vVertex.push_back({b, vertB.normal, vertB.texture}); // T2 PA
    vVertex.push_back({B, vertB.normal, texB});          // T2 PB
    vVertex.push_back({A, vertA.normal, texA});          // T2 PC
    //_vTriangle.push_back(new Triangle(last++, last++, last++, normal));
    Triangle* th2 = new Triangle(last++, last++, last++, normal);
    th2->beenUsedAsSplitter = _pTriangle->beenUsedAsSplitter;
    _vTriangle.push_front(th2); // TODO :Testar se é isto mesmo

    // // -- T3 Triangle T3(A, B, c);
    vVertex.push_back({A, vertA.normal, texA});          // T3 PA
    vVertex.push_back({B, vertB.normal, texB});          // T3 PB
    vVertex.push_back({c, vertC.normal, vertC.texture}); // T3 PC
    //_vTriangle.push_back(new Triangle(last++, last++, last++, normal));
    Triangle* th3 = new Triangle(last++, last++, last++, normal);
    th3->beenUsedAsSplitter = _pTriangle->beenUsedAsSplitter;
    _vTriangle.push_front(th3); // TODO :Testar se é isto mesmo

    // Remove orininal
    delete _pTriangle;
    _pTriangle = nullptr;
}

bool BspTree::isConvex(std::list<Triangle*>& _vTriangle, Triangle* _poly) {

    if (_vTriangle.size() <= 1)
        return false;

    Triangle* th1 = nullptr;
    Triangle* th2 = nullptr;
    float sameDir = 0.0;

    for (std::list<Triangle*>::iterator i = _vTriangle.begin(); i != _vTriangle.end(); i++) {

        th1 = (*i);

        for (std::list<Triangle*>::iterator j = i; j != _vTriangle.end(); j++) {

            th2 = (i != j) ? (*j) : _poly; // Test hyperplane is Convex too in coincident index

            glm::vec3 u = th1->getNormal();
            glm::vec3 v = th2->getNormal();
            float val = glm::dot(u, v);
            if (val > 0.0f) { // if not convex test if is coplanar
                glm::vec3 result;
                Plane alpha(th1->position(vVertex, 0), th1->getNormal());
                if (alpha.classifyPoly(th2->position(vVertex, 0), th2->position(vVertex, 1), th2->position(vVertex, 2), &result) !=
                    SIDE::CP_ONPLANE)
                    return false;

                // test if faces has oposites directions aka: convex
                if (alpha.collinearNormal(th1->getNormal()) == false)
                    return false;
            }
        }
    }

    return true;
}

BSPTreeNode* BspTree::build(std::list<Triangle*>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    Triangle* best = selectBestSplitter(_vTriangle);
    BSPTreeNode* tree = new BSPTreeNode(Plane(best->position(vVertex, 0), best->getNormal()));

    std::list<Triangle*> front_list;
    std::list<Triangle*> back_list;

    Triangle* poly = nullptr;

    while (_vTriangle.empty() == false) {

        poly = _vTriangle.back();
        _vTriangle.pop_back();
        glm::vec3 result;
        SIDE clipTest = tree->hyperPlane.classifyPoly(poly->position(vVertex, 0), // PA old poly.vertex[0].position
                                                      poly->position(vVertex, 1), // PB
                                                      poly->position(vVertex, 2), // PC
                                                      &result);                   // Clip Test Result (A,B,C)
        switch (clipTest) {
            case SIDE::CP_BACK:
                back_list.push_front(poly);
                break;
            case SIDE::CP_FRONT:
                front_list.push_front(poly);
                break;
            case SIDE::CP_ONPLANE:
                tree->addPolygon(poly);
                break;
            default:
                splitTriangle(result, poly, tree->hyperPlane, _vTriangle);
                break;
        }
    }

    // Verify if all triangles front are convex
    if (isConvex(front_list, poly) == true) {

        // next front only have conves set
        BSPTreeNode* convex = new BSPTreeNode(tree->hyperPlane);
        convex->addIndexPolygon(front_list);
        convex->front = new BSPTreeNode(tree->hyperPlane);
        convex->front->isLeaf = true;
        convex->front->isSolid = false;
        convex->back = new BSPTreeNode(tree->hyperPlane);
        convex->back->isLeaf = true;
        convex->back->isSolid = true;
        tree->front = convex;
    } else {
        tree->front = build(front_list);
    }

    tree->back = build(back_list);

    // leaf sem poligonos apenas para saber se solido ou vazio
    if (tree->front == nullptr) {
        tree->front = new BSPTreeNode(tree->hyperPlane);
        tree->front->isLeaf = true;
        tree->front->isSolid = false;
    }

    if (tree->back == nullptr) {
        tree->back = new BSPTreeNode(tree->hyperPlane);
        tree->back->isLeaf = true;
        tree->back->isSolid = true;
    }

    return tree;
}

BSPTreeNode* BspTree::buildLeafy(std::list<Triangle*>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    Triangle* poly = nullptr;
    std::list<Triangle*> front_list;
    std::list<Triangle*> back_list;

    Triangle* best = selectBestSplitter(_vTriangle);
    BSPTreeNode* tree = new BSPTreeNode(Plane(best->position(vVertex, 0), best->getNormal()));

    while (_vTriangle.empty() == false) {

        poly = _vTriangle.back();
        _vTriangle.pop_back();
        glm::vec3 result;
        SIDE clipTest = tree->hyperPlane.classifyPoly(poly->position(vVertex, 0), // PA old poly.vertex[0].position
                                                      poly->position(vVertex, 1), // PB
                                                      poly->position(vVertex, 2), // PC
                                                      &result);                   // Clip Test Result (A,B,C)
        switch (clipTest) {
            case SIDE::CP_BACK:
                back_list.push_front(poly);
                break;
            case SIDE::CP_FRONT:
                front_list.push_front(poly);
                break;
            case SIDE::CP_ONPLANE: {
                if (tree->hyperPlane.collinearNormal(poly->getNormal()) == true)
                    front_list.push_front(poly);
                else
                    back_list.push_front(poly);
            } break;
            default:
                splitTriangle(result, poly, tree->hyperPlane, _vTriangle);
                break;
        }
    }

    unsigned int count = 0;
    for (auto th : front_list) {
        if (th->beenUsedAsSplitter == false)
            count++;
        // break; //TODO: testar se mais eficiente a contar todos
    }

    if (count == 0) {
        // BSPTreeNode* convex = new BSPTreeNode(partition);
        // convex->isLeaf = true;
        // convex->isSolid = false;
        tree->addIndexPolygon(front_list);
        tree->isSolid = false;
        tree->isLeaf = true;
    } else {
        tree->front = buildLeafy(front_list);
    }

    tree->back = buildLeafy(back_list);
    if (tree->back == nullptr) {
        if (tree->pLeaf == nullptr) {
            BSPTreeNode* solid = new BSPTreeNode(tree->hyperPlane);
            solid->isSolid = true;
            tree->isLeaf = false;
            tree->back = solid;
        }
    }

    return tree;
}

bool BspTree::lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree) {
    float temp;
    glm::vec3 intersection;
    if (tree->isLeaf == true) {
        return !tree->isSolid;
    }

    SIDE PointA = tree->hyperPlane.classifyPoint(Start);
    SIDE PointB = tree->hyperPlane.classifyPoint(End);

    if (PointA == SIDE::CP_ONPLANE && PointB == SIDE::CP_ONPLANE) {
        return lineOfSight(Start, End, tree->front);
    }

    if (PointA == SIDE::CP_FRONT && PointB == SIDE::CP_BACK) {
        tree->hyperPlane.intersect(Start, End, &intersection, &temp);
        return lineOfSight(Start, &intersection, tree->front) && lineOfSight(End, &intersection, tree->back);
    }

    if (PointA == SIDE::CP_BACK && PointB == SIDE::CP_FRONT) {
        tree->hyperPlane.intersect(Start, End, &intersection, &temp);
        return lineOfSight(End, &intersection, tree->front) && lineOfSight(Start, &intersection, tree->back);
    }

    // if we get here one of the points is on the hyperPlane
    if (PointA == SIDE::CP_FRONT || PointB == SIDE::CP_FRONT) {
        return lineOfSight(Start, End, tree->front);
    } else {
        return lineOfSight(Start, End, tree->back);
    }
    return true;
}

} // namespace Chimera
