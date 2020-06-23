#include "chimera/render/PVS.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Leaf::Leaf() {}

Leaf::~Leaf() {
    while (index.empty() == false) {
        index.clear();
    }
}

void Leaf::addFace(uint32_t face, uint32_t _a, uint32_t _b, uint32_t _c) {
    index.push_back(_a);
    index.push_back(_b);
    index.push_back(_c);
    faces.push_back(face);
    // TODO: implementar acerto do AABB
}

BSPTreeNode::BSPTreeNode(const Plane& _hyperPlane)
    : hyperPlane(_hyperPlane), front(nullptr), back(nullptr), pLeaf(nullptr), isSolid(false) {}

BSPTreeNode::~BSPTreeNode() { this->destroy(); }

void BSPTreeNode::addIndexPolygon(std::list<Triangle*>& _vTriangle) {

    pLeaf = new Leaf;
    while (_vTriangle.empty() == false) {
        Triangle* convPoly = _vTriangle.back();
        _vTriangle.pop_back();
        pLeaf->addFace(convPoly->getSerial(), convPoly->p[0], convPoly->p[1], convPoly->p[2]);

        delete convPoly;
        convPoly = nullptr;
    }
}

void BSPTreeNode::destroy() {

    if (pLeaf != nullptr) {
        delete pLeaf;
        pLeaf = nullptr;
    }
}

template <class T> void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

PVS::PVS() {
    root = nullptr;
    resultVertex = nullptr;
    logdata = false;
}

void PVS::createSequencial(std::vector<Chimera::VertexData>& _vVertex) {

    std::list<Triangle*> vTris;
    vVertex = _vVertex;

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

    root = bsptreeBuild(vTris);
}

void PVS::createIndexed(std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex) {

    std::list<Triangle*> vTris;
    vVertex = _vVertex;

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

    root = bsptreeBuild(vTris);
}

void PVS::destroy() { collapse(root); }

void PVS::collapse(BSPTreeNode* tree) {

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

void PVS::drawPolygon(BSPTreeNode* tree, bool frontSide) {

    if (tree->pLeaf == nullptr)
        return;

    if (logdata == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Convex: %ld", tree->pLeaf->index.size());

    for (auto index : tree->pLeaf->index)
        resultVertex->push_back(vVertex[index]);
}

void PVS::traverseTree(BSPTreeNode* tree, glm::vec3* pos) {
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

// void PVS::traverseTree(BSPTreeNode* tree, glm::vec3* pos) {
//     // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html

//     BSPTreeNode* node = tree;
//     while (true) {
//         switch (node->hyperPlane.classifyPoint(pos)) {
//             case SIDE::CP_FRONT: {
//                 if (node->isLeaf == true) {
//                     drawPolygon(node, true);
//                     return;
//                 }
//                 node = node->front;
//             } break;
//             case SIDE::CP_BACK: {
//                 if (node->isSolid == true)
//                     return;

//                 node = node->back;
//             } break;
//             case SIDE::CP_ONPLANE: {
//                 if (node->isLeaf == true) {
//                     drawPolygon(node, true);
//                     return;
//                 }
//                 node = node->front;
//             } break;
//         }
//     }
// }

void PVS::render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool _logData) {
    logdata = _logData;
    resultVertex = _pOutVertex;
    traverseTree(root, eye);
}

Plane PVS::selectBestSplitter(std::list<Triangle*>& _vTriangle) {

    if (_vTriangle.size() == 0)
        return Plane();

    std::list<Triangle*>::iterator selectedPoly;
    unsigned int bestScore = 100000; // just set to a very high value to begin
    glm::vec3 temp;                  // inutil

    for (std::list<Triangle*>::iterator indice_splitter = _vTriangle.begin(); indice_splitter != _vTriangle.end(); indice_splitter++) {

        Triangle* th = (*indice_splitter); //_vTriangle[indice_splitter];
        if (th->beenUsedAsSplitter == true)
            continue;

        Plane hyperPlane(vPosVal(th, 0), th->getNormal());

        long long score, splits, backfaces, frontfaces;
        score = splits = backfaces = frontfaces = 0;

        for (std::list<Triangle*>::iterator indice_current = _vTriangle.begin(); indice_current != _vTriangle.end(); indice_current++) {

            if (indice_current != indice_splitter) {

                Triangle* currentPoly = (*indice_current);
                SIDE result = hyperPlane.classifyPoly(vPosVal(currentPoly, 0), // PA
                                                      vPosVal(currentPoly, 1), // PB
                                                      vPosVal(currentPoly, 2), // PC
                                                      &temp);                  // Clip Test Result (A,B,C)
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
            selectedPoly = indice_splitter;
        }

    } // end while splitter

    Triangle* th = (*selectedPoly);
    th->beenUsedAsSplitter = true;
    return Plane(vPosVal(th, 0), th->getNormal());
}

void PVS::splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle) {

    // Proporcao de textura (0.0 a 1.0)
    float propAC = 0.0;
    float propBC = 0.0;

    // ultima posicao de indice de vertices
    unsigned int last = vVertex.size();

    // Vertex dos triangulos a serem normalizados
    Chimera::VertexData vertA, vertB, vertC;

    // Pega pontos posicao original e inteseccao
    glm::vec3 A, B;
    glm::vec3 a = vPosVal(_pTriangle, 0);
    glm::vec3 b = vPosVal(_pTriangle, 1);
    glm::vec3 c = vPosVal(_pTriangle, 2);

    // Normaliza Triangulo para que o corte do triangulo esteja nos segmentos de reta CA e CB (corte em a e b)
    if (fx.x * fx.z >= 0) {             // corte em a e c (rotaciona pontos sentido horario) ABC => BCA
        swapFace(b, c);                 //   troca b com c
        swapFace(a, b);                 //   troca a com b
        vertA = vVerVal(_pTriangle, 2); //   old c
        vertB = vVerVal(_pTriangle, 0); //   old a
        vertC = vVerVal(_pTriangle, 1); //   old b

    } else if (fx.y * fx.z >= 0) {      // corte em b e c (totaciona pontos sentido anti-horario)  ABC => CAB
        swapFace(a, c);                 //   troca A com C
        swapFace(a, b);                 //   torca a com b
        vertA = vVerVal(_pTriangle, 1); //   old b
        vertB = vVerVal(_pTriangle, 2); //   old c
        vertC = vVerVal(_pTriangle, 0); //   old a

    } else {                            // Cortre em a e b (pontos posicao original)
        vertA = vVerVal(_pTriangle, 0); //   old a
        vertB = vVerVal(_pTriangle, 1); //   old b
        vertC = vVerVal(_pTriangle, 2); //   old c
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

BSPTreeNode* PVS::bsptreeBuild(std::list<Triangle*>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    Plane partition = selectBestSplitter(_vTriangle);
    BSPTreeNode* tree = new BSPTreeNode(partition);

    std::list<Triangle*> front_list;
    std::list<Triangle*> back_list;

    Triangle* poly = nullptr;

    while (_vTriangle.empty() == false) {

        poly = _vTriangle.back();
        _vTriangle.pop_back();
        glm::vec3 result;
        SIDE clipTest = tree->hyperPlane.classifyPoly(vPosVal(poly, 0), // PA old poly.vertex[0].position
                                                      vPosVal(poly, 1), // PB
                                                      vPosVal(poly, 2), // PC
                                                      &result);         // Clip Test Result (A,B,C)
        switch (clipTest) {
            case SIDE::CP_BACK:
                back_list.push_front(poly);
                break;
            case SIDE::CP_FRONT:
                front_list.push_front(poly);
                break;
            case SIDE::CP_ONPLANE: {
                if (partition.collinearNormal(poly->getNormal()) == true)
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
    } else {
        tree->front = bsptreeBuild(front_list);
    }

    tree->back = bsptreeBuild(back_list);
    if (tree->back == nullptr) {
        if (tree->pLeaf == nullptr) {
            BSPTreeNode* solid = new BSPTreeNode(partition);
            solid->isSolid = true;
            tree->back = solid;
        }
    }

    return tree;
}

bool PVS::lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree) {
    float temp;
    glm::vec3 intersection;
    if (tree->pLeaf != nullptr) {
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