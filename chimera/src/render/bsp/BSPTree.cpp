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

void BspTree::create(std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex) {

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
            vTris.push_back(new Triangle(pa, pb, pc, normal));
        }
    }

    root = buildLeafy(vTris);
}

void BspTree::destroy() {

    while (!vpLeaf.empty()) {

        Leaf* pLeaf = vpLeaf.back();
        vpLeaf.pop_back();

        delete pLeaf;
        pLeaf = nullptr;
    }

    collapse(root);
}

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

    if (tree->isLeaf == false)
        return;

    if (logdata == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Convex: %ld", vpLeaf[tree->leafIndex]->index.size()); // tree->pLeaf->index.size());

    for (auto index : vpLeaf[tree->leafIndex]->index) // for (auto index : tree->pLeaf->index)
        resultVertex->push_back(vVertex[index]);
}

void BspTree::traverseTree(BSPTreeNode* tree, glm::vec3* pos) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    if (tree->isSolid == true)
        return;

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

    if (selectedTriangle != nullptr)
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

    Triangle* th1 = new Triangle(last++, last++, last++, normal);
    th1->beenUsedAsSplitter = _pTriangle->beenUsedAsSplitter;
    _vTriangle.push_front(th1);

    //-- T2 Triangle T2(b, B, A);
    vVertex.push_back({b, vertB.normal, vertB.texture}); // T2 PA
    vVertex.push_back({B, vertB.normal, texB});          // T2 PB
    vVertex.push_back({A, vertA.normal, texA});          // T2 PC

    Triangle* th2 = new Triangle(last++, last++, last++, normal);
    th2->beenUsedAsSplitter = _pTriangle->beenUsedAsSplitter;
    _vTriangle.push_front(th2);

    // -- T3 Triangle T3(A, B, c);
    vVertex.push_back({A, vertA.normal, texA});          // T3 PA
    vVertex.push_back({B, vertB.normal, texB});          // T3 PB
    vVertex.push_back({c, vertC.normal, vertC.texture}); // T3 PC

    Triangle* th3 = new Triangle(last++, last++, last++, normal);
    th3->beenUsedAsSplitter = _pTriangle->beenUsedAsSplitter;
    _vTriangle.push_front(th3);

    // Remove orininal
    delete _pTriangle;
    _pTriangle = nullptr;
}

BSPTreeNode* BspTree::buildLeafy(std::list<Triangle*>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    std::list<Triangle*> front_list;
    std::list<Triangle*> back_list;
    Triangle* poly = nullptr;
    BSPTreeNode* tree = nullptr;

    Triangle* best = selectBestSplitter(_vTriangle);
    if (best != nullptr) {
        tree = new BSPTreeNode(Plane(best->position(vVertex, 0), best->getNormal()));
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
    } else {
        // FIXME: used only to test broken vertexdata map
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Falha no BackFace");
        bool primeiro = true;
        while (_vTriangle.empty() == false) {
            poly = _vTriangle.back();
            _vTriangle.pop_back();
            if (primeiro == true) {
                tree = new BSPTreeNode(Plane(poly->position(vVertex, 0), poly->getNormal()));
                primeiro = false;
            }
            front_list.push_front(poly);
        }
    }

    unsigned int count = 0;
    for (auto th : front_list) {
        if (th->beenUsedAsSplitter == false)
            count++;
    }

    if (count == 0) {
        createLeafy(tree, front_list);
    } else {
        tree->front = buildLeafy(front_list);
    }

    tree->back = buildLeafy(back_list);
    if (tree->back == nullptr) {
        if (tree->isLeaf == false) {
            BSPTreeNode* solid = new BSPTreeNode(tree->hyperPlane);
            solid->isSolid = true;
            tree->back = solid;
        }
    }

    return tree;
}

void BspTree::createLeafy(BSPTreeNode* tree, std::list<Triangle*>& listConvexTriangle) {

    Leaf* pLeaf = new Leaf();

    while (listConvexTriangle.empty() == false) {
        Triangle* convPoly = listConvexTriangle.back();
        listConvexTriangle.pop_back();
        pLeaf->addFace(convPoly->getSerial(), convPoly->p[0], convPoly->p[1], convPoly->p[2]);

        delete convPoly;
        convPoly = nullptr;
    }

    tree->leafIndex = vpLeaf.size();
    vpLeaf.push_back(pLeaf);

    tree->isSolid = false;
    tree->isLeaf = true;
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
