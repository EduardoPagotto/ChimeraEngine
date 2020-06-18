#include "chimera/render/PVS.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

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

    std::vector<Triangle*> vTris;
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

    std::vector<Triangle*> vTris;
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

    while (tree->polygons.empty() == false) {
        Triangle* poly = tree->polygons.back();
        tree->polygons.pop_back();
        delete poly;
        poly = nullptr;
    }

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

    if (logdata == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Convex: %ld", tree->polygons.size());

    for (auto it = tree->polygons.begin(); it != tree->polygons.end(); it++) {
        Triangle* t = (*it);

        resultVertex->push_back(vVerVal(t, 0));
        resultVertex->push_back(vVerVal(t, 1));
        resultVertex->push_back(vVerVal(t, 2));

        // FIXME: remover depois de concluir o algoritmo
        if (logdata == true) {
            if (frontSide == true)
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "    Face F: %d", t->getSerial());
            else
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "    Face B: %d", t->getSerial());
        }
    }
}

void PVS::traverseTree(BSPTreeNode* tree, glm::vec3* pos) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    if (tree->isSolid == true)
        return;

    SIDE result = tree->hyperPlane.classifyPoint(pos);
    switch (result) {
        case SIDE::CP_FRONT: {
            if (tree->isLeaf == true) {
                drawPolygon(tree, true);
                return;
            } else {
                traverseTree(tree->front, pos);
            }
        } break;
        case SIDE::CP_BACK: {
            if (tree->isSolid == true)
                return;
            else {
                traverseTree(tree->back, pos);
                traverseTree(tree->front, pos);
            }
        } break;
        case SIDE::CP_ONPLANE: {
            if (tree->isLeaf == true) {
                drawPolygon(tree, true);
                return;
            } else {
                traverseTree(tree->front, pos);
            }
        }
        default:
            break;
    }
}
// void PVS::traverseTree(BSPTreeNode* tree, glm::vec3* pos) {
//     // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
//     if (tree == nullptr)
//         return;
//     if (tree->isSolid == true)
//         return;

//     SIDE result = tree->hyperPlane.classifyPoint(pos);
//     switch (result) {
//         case SIDE::CP_FRONT:
//             traverseTree(tree->back, pos);
//             drawPolygon(tree, true);
//             traverseTree(tree->front, pos);
//             break;
//         case SIDE::CP_BACK:
//             traverseTree(tree->front, pos);
//             drawPolygon(tree, false); // Elimina o render do back-face
//             traverseTree(tree->back, pos);
//             break;
//         default: // SIDE::CP_ONPLANE
//             // the eye point is on the partition hyperPlane...
//             traverseTree(tree->front, pos);
//             traverseTree(tree->back, pos);
//             break;
//     }
// }

void PVS::render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool _logData) {
    logdata = _logData;
    resultVertex = _pOutVertex;
    traverseTree(root, eye);
}

Plane PVS::selectBestSplitter(std::vector<Triangle*>& _vTriangle) {

    if (_vTriangle.size() == 0)
        return Plane();

    unsigned int selectedPoly = 0;
    unsigned int bestScore = 100000; // just set to a very high value to begin
    glm::vec3 temp;                  // inutil

    for (unsigned indice_splitter = 0; indice_splitter < _vTriangle.size(); indice_splitter++) {

        Triangle* th = _vTriangle[indice_splitter];
        if (th->beenUsedAsSplitter == true)
            continue;

        Plane hyperPlane(vPosVal(th, 0), th->getNormal());

        long long score, splits, backfaces, frontfaces;
        score = splits = backfaces = frontfaces = 0;

        for (unsigned indice_current = 0; indice_current < _vTriangle.size(); indice_current++) {

            if (indice_current != indice_splitter) {

                Triangle* currentPoly = _vTriangle[indice_current];
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

    } // end while splitter == null

    Triangle* th = _vTriangle[selectedPoly];
    th->beenUsedAsSplitter = true;
    return Plane(vPosVal(th, 0), th->getNormal());
}

void PVS::splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::vector<Triangle*>& _vTriangle) {

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
    _vTriangle.push_back(th1); // TODO :Testar se é isto mesmo

    // //-- T2 Triangle T2(b, B, A);
    vVertex.push_back({b, vertB.normal, vertB.texture}); // T2 PA
    vVertex.push_back({B, vertB.normal, texB});          // T2 PB
    vVertex.push_back({A, vertA.normal, texA});          // T2 PC
    //_vTriangle.push_back(new Triangle(last++, last++, last++, normal));
    Triangle* th2 = new Triangle(last++, last++, last++, normal);
    th2->beenUsedAsSplitter = _pTriangle->beenUsedAsSplitter;
    _vTriangle.push_back(th2); // TODO :Testar se é isto mesmo

    // // -- T3 Triangle T3(A, B, c);
    vVertex.push_back({A, vertA.normal, texA});          // T3 PA
    vVertex.push_back({B, vertB.normal, texB});          // T3 PB
    vVertex.push_back({c, vertC.normal, vertC.texture}); // T3 PC
    //_vTriangle.push_back(new Triangle(last++, last++, last++, normal));
    Triangle* th3 = new Triangle(last++, last++, last++, normal);
    th3->beenUsedAsSplitter = _pTriangle->beenUsedAsSplitter;
    _vTriangle.push_back(th3); // TODO :Testar se é isto mesmo
}

// bool PVS::isConvex(std::vector<Triangle*>& _vTriangle, Triangle* _poly) {

//     if (_vTriangle.size() <= 1)
//         return false;

//     Triangle* th1 = nullptr;
//     Triangle* th2 = nullptr;
//     float sameDir = 0.0;

//     for (unsigned i = 0; i < _vTriangle.size(); i++) {

//         th1 = _vTriangle[i];

//         for (unsigned j = i; j < _vTriangle.size(); j++) {

//             th2 = (i != j) ? _vTriangle[j] : _poly; // Test hyperplane is Convex too in coincident index

//             glm::vec3 u = th1->getNormal();
//             glm::vec3 v = th2->getNormal();
//             float val = glm::dot(u, v);
//             if (val > 0.0f) { // if not convex test if is coplanar
//                 glm::vec3 result;
//                 Plane alpha(vPosVal(th1, 0), th1->getNormal());
//                 if (alpha.classifyPoly(vPosVal(th2, 0), vPosVal(th2, 1), vPosVal(th2, 2), &result) != SIDE::CP_ONPLANE)
//                     return false;

//                 // test if faces has oposites directions aka: convex
//                 if (alpha.collinearNormal(th1->getNormal()) == false)
//                     return false;
//             }
//         }
//     }

//     return true;
// }

BSPTreeNode* PVS::bsptreeBuild(std::vector<Triangle*>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    Plane partition = selectBestSplitter(_vTriangle);
    BSPTreeNode* tree = new BSPTreeNode(partition);

    std::vector<Triangle*> front_list;
    std::vector<Triangle*> back_list;

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
                back_list.push_back(poly);
                break;
            case SIDE::CP_FRONT:
                front_list.push_back(poly);
                break;
            case SIDE::CP_ONPLANE: {
                if (partition.collinearNormal(poly->getNormal()) == false)
                    front_list.push_back(poly);
                else
                    back_list.push_back(poly);
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
        while (front_list.empty() == false) {
            Triangle* convPoly = front_list.back();
            front_list.pop_back();
            tree->polygons.push_back(convPoly);
            // convex->polygons.push_back(convPoly);
        }
        // tree->front = convex;
        tree->isLeaf = true;
        tree->isSolid = false;
    } else {
        tree->front = bsptreeBuild(front_list);
    }

    tree->back = bsptreeBuild(back_list);

    if (back_list.empty() == true) {
        if (tree->polygons.size() == 0) {
            BSPTreeNode* solid = new BSPTreeNode(partition);
            solid->isLeaf = false;
            solid->isSolid = true;
            tree->back = solid;
        }
    }

    return tree;
}

bool PVS::lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree) {
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

// namespace Chimera {

// PVS::PVS() {
//     NumberOfPolygons = 0;
//     NumberOfNodes = 0;
//     NumberOfLeafs = 0;
//     NumberOfPlanes = 0;
//     NumberOfPortals = 0;
// }

// unsigned int PVS::selectBestSplitter(std::vector<Triangle*>& _poliyList) {

//     if (_poliyList.size() == 0)
//         return 0;

//     unsigned int selectedPoly = 0;
//     unsigned int bestScore = 100000; // just set to a very high value to begin
//     glm::vec3 temp;                  // inutil

//     for (unsigned indice_splitter = 0; indice_splitter < _poliyList.size(); indice_splitter++) {

//         if (_poliyList[indice_splitter]->beenUsedAsSplitter == true)
//             continue;

//         Triangle* th = _poliyList[indice_splitter];
//         Plane hyperPlane(vPosVal(th, 0), th->getNormal());

//         long long score, splits, backfaces, frontfaces;
//         score = splits = backfaces = frontfaces = 0;

//         for (unsigned indice_current = 0; indice_current < _poliyList.size(); indice_current++) {

//             if (indice_current != indice_splitter) {

//                 Triangle* currentPoly = _poliyList[indice_current];
//                 SIDE result = hyperPlane.classifyPoly(vPosVal(currentPoly, 0), vPosVal(currentPoly, 1), vPosVal(currentPoly, 2), &temp);
//                 switch (result) {
//                     case SIDE::CP_ONPLANE:
//                         break;
//                     case SIDE::CP_FRONT:
//                         frontfaces++;
//                         break;
//                     case SIDE::CP_BACK:
//                         backfaces++;
//                         break;
//                     case SIDE::CP_SPANNING:
//                         splits++;
//                         break;
//                     default:
//                         break;
//                 }
//             }
//         } // end while current poly

//         score = abs(frontfaces - backfaces) + (splits * 8);

//         if (score < bestScore) {
//             bestScore = score;
//             selectedPoly = indice_splitter;
//         }
//     }

//     Triangle* th = _poliyList[selectedPoly];
//     th->beenUsedAsSplitter = true;
//     Plane* p = new Plane();

//     p->set(vPosVal(th, 0), th->getNormal());
//     this->planes.push_back(p);

//     return this->planes.size() - 1; // plane create using poly selected above
// }

// void PVS::buildBspTree(long Node, std::vector<Triangle*>& PolyList) {

//     nodeArray[Node].plane = selectBestSplitter(PolyList);
//     nodeArray[Node].boundingBox.setPosition(glm::vec3(0.0f), glm::vec3(4000));

//     std::vector<Triangle*> FrontList;
//     std::vector<Triangle*> BackList;

//     // polyTest = PolyList;
//     // while (PolyList.empty() == false) {
//     float result;
//     glm::vec3 resultClip(0.0f);

//     for (auto it = PolyList.begin(); it != PolyList.end(); it++) {

//         Triangle* polyTest = *it;

//         SIDE clipTest = planeArray[nodeArray[Node].plane].classifyPoly(vPosVal(polyTest, 0), // PA old poly.vertex[0].position
//                                                                        vPosVal(polyTest, 1), // PB
//                                                                        vPosVal(polyTest, 2), // PC
//                                                                        &resultClip);         // Clip Test Result (A,B,C)

//         switch (clipTest) {
//             case SIDE::CP_ONPLANE: {
//                 if (planeArray[nodeArray[Node].plane].collinearNormal(polyTest->getNormal()) == false) {
//                     // polyTest->Next = FrontList;
//                     FrontList.push_back(polyTest);
//                 } else {
//                     // polyTest->Next = BackList;
//                     BackList.push_back(polyTest);
//                 }
//             } break;

//             case SIDE::CP_FRONT:
//                 // polyTest->Next = FrontList;
//                 FrontList.push_back(polyTest);
//                 break;

//             case SIDE::CP_BACK:
//                 // polyTest->Next = BackList;
//                 BackList.push_back(polyTest);
//                 break;
//             case SIDE::CP_SPANNING:
//                 // TODO: DEPOIS!!!! muita merda aqui!!!
//                 break;

//             default:
//                 break;
//         }
//     }

//     int count = 0;
//     for (Triangle* th : FrontList) {
//         if (th->beenUsedAsSplitter == false)
//             count++;
//     }

//     // CalculateBox(&NodeArray[Node].BoundingBox, FrontList);
//     AABB LeafBox = nodeArray[Node].boundingBox;
//     // CalculateBox(&NodeArray[Node].BoundingBox, BackList);

//     if (count == 0) {
//         // POLYGON* Iterator = FrontList;
//         // POLYGON* Temp;
//         // LeafArray[NumberOfLeafs].StartPolygon = NumberOfPolygons;

//         // No loop adiciona os vertices a lista de desenho
//         // while (Iterator != NULL) {
//         //     // PolygonArray[NumberOfPolygons] = *Iterator;
//         //     // IncreaseNumberOfPolygons();
//         //     // Temp = Iterator;
//         //     // Iterator = Iterator->Next;
//         //     // delete Temp; // we use the normal delete operator here because we dont want to delete the actual vertexLists and index
//         //     lists
//         //     //              // because we are still referenceing them in the Polygon Array
//         // }
//         LeafArray[NumberOfLeafs].EndPolygon = NumberOfPolygons;
//         LeafArray[NumberOfLeafs].BoundingBox = LeafBox;
//         nodeArray[Node].front = NumberOfLeafs;
//         nodeArray[Node].IsLeaf = 1;
//         // IncreaseNumberOfLeafs();
//     } else {
//         nodeArray[Node].IsLeaf = 0;
//         nodeArray[Node].front = NumberOfNodes + 1;
//         // IncreaseNumberOfNodes();
//         buildBspTree(NumberOfNodes, FrontList);
//     }

//     if (BackList.empty() == true) {
//         nodeArray[Node].back = -1;
//     } else {
//         nodeArray[Node].back = NumberOfNodes + 1;
//         // IncreaseNumberOfNodes();
//         buildBspTree(NumberOfNodes, BackList);
//     }
// }

// } // namespace Chimera