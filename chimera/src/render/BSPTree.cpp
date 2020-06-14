#include "chimera/render/BSPTree.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

template <class T> void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

glm::vec3 aprox(const glm::vec3& dado) {
    return glm::vec3((fabs(dado.x) < EPSILON) ? 0.0f : dado.x,  // X
                     (fabs(dado.y) < EPSILON) ? 0.0f : dado.y,  // Y
                     (fabs(dado.z) < EPSILON) ? 0.0f : dado.z); // Z
}

BspTree::BspTree() {
    root = nullptr;
    resultVertex = nullptr;
    logdata = false;
}

void BspTree::create(std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex) {

    std::vector<Triangle> vTris;
    vVertex = _vVertex;

    for (unsigned int indice = 0; indice < _vIndex.size(); indice += 3) {
        unsigned int pa = _vIndex[indice];
        unsigned int pb = _vIndex[indice + 1];
        unsigned int pc = _vIndex[indice + 2];

        // Calcula Normal Face
        glm::vec3 acc = vVertex[pa].normal + vVertex[pb].normal + vVertex[pc].normal;
        glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
        vTris.push_back(Triangle(pa, pb, pc, normal));
    }

    root = bsptreeBuild(vTris);
}

void BspTree::drawPolygon(BSPTreeNode* tree, bool frontSide) {

    if (logdata == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Convex: %ld", tree->polygons.size());

    for (auto it = tree->polygons.begin(); it != tree->polygons.end(); it++) {
        Triangle t = (*it);

        resultVertex->push_back(vVerVal(t, 0));
        resultVertex->push_back(vVerVal(t, 1));
        resultVertex->push_back(vVerVal(t, 2));

        // FIXME: remover depois de concluir o algoritmo
        if (logdata == true) {
            if (frontSide == true)
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "    Face F: %d", t.getSerial());
            else
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "    Face B: %d", t.getSerial());
        }
    }
}

void BspTree::traverseTree(BSPTreeNode* tree, glm::vec3* pos) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    // no de indicador de final/solido
    if (tree->isLeaf == true)
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
            drawPolygon(tree, false);
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

unsigned int BspTree::selectBestSplitter(std::vector<Triangle>& _vTriangle) {

    if (_vTriangle.size() == 0)
        return 0;

    unsigned int selectedPoly = 0;
    unsigned int bestScore = 100000; // just set to a very high value to begin
    glm::vec3 temp;                  // inutil

    for (unsigned indice_splitter = 0; indice_splitter < _vTriangle.size(); indice_splitter++) {

        Triangle th = _vTriangle[indice_splitter];
        Plane hyperPlane;
        hyperPlane.set(vPosVal(th, 0), th.getNormal());

        long long score, splits, backfaces, frontfaces;
        score = splits = backfaces = frontfaces = 0;

        for (unsigned indice_current = 0; indice_current < _vTriangle.size(); indice_current++) {

            if (indice_current != indice_splitter) {

                Triangle currentPoly = _vTriangle[indice_current];
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
    return selectedPoly;
}

void BspTree::splitTriangle(const glm::vec3& fx, Triangle& _pTriangle, Plane& hyperPlane, std::vector<Triangle>& _vTriangle) {

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
    vVertex.push_back({a, vertA.normal, vertA.texture});            // T1 PA
    vVertex.push_back({b, vertB.normal, vertB.texture});            // T1 PB
    vVertex.push_back({A, vertA.normal, texA});                     // T1 PC
    _vTriangle.push_back(Triangle(last++, last++, last++, normal)); // Triangle T1(last++, last++, last++, *vVertex);

    // //-- T2 Triangle T2(b, B, A);
    vVertex.push_back({b, vertB.normal, vertB.texture});            // T2 PA
    vVertex.push_back({B, vertB.normal, texB});                     // T2 PB
    vVertex.push_back({A, vertA.normal, texA});                     // T2 PC
    _vTriangle.push_back(Triangle(last++, last++, last++, normal)); // Triangle T2(last++, last++, last++, *vVertex);

    // // -- T3 Triangle T3(A, B, c);
    vVertex.push_back({A, vertA.normal, texA});                     // T3 PA
    vVertex.push_back({B, vertB.normal, texB});                     // T3 PB
    vVertex.push_back({c, vertC.normal, vertC.texture});            // T3 PC
    _vTriangle.push_back(Triangle(last++, last++, last++, normal)); // Triangle T3(last++, last++, last++, *vVertex);
}

BSPTreeNode* BspTree::bsptreeBuild(std::vector<Triangle>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    // balanceador
    unsigned int bether_index = selectBestSplitter(_vTriangle);
    Triangle partition = _vTriangle[bether_index];
    BSPTreeNode* tree = new BSPTreeNode(vPosVal(partition, 0), partition.getNormal());

    std::vector<Triangle> front_list;
    std::vector<Triangle> back_list;

    while (_vTriangle.empty() == false) {

        Triangle poly = _vTriangle.back();
        _vTriangle.pop_back();
        glm::vec3 result;
        SIDE teste = tree->hyperPlane.classifyPoly(vPosVal(poly, 0), // PA old poly.vertex[0].position
                                                   vPosVal(poly, 1), // PB
                                                   vPosVal(poly, 2), // PC
                                                   &result);         // Clip Test Result (A,B,C)
        switch (teste) {
            case SIDE::CP_BACK:
                back_list.push_back(poly);
                break;
            case SIDE::CP_FRONT:
                front_list.push_back(poly);
                break;
            case SIDE::CP_ONPLANE:
                tree->polygons.push_back(poly);
                break;
            default:
                splitTriangle(result, poly, tree->hyperPlane, _vTriangle);
                break;
        }
    }

    tree->front = bsptreeBuild(front_list);
    tree->back = bsptreeBuild(back_list);

    // leaf sem poligonos apenas para saber se solido ou vazio
    if (tree->front == nullptr) {
        tree->front = new BSPTreeNode(vPosVal(partition, 0), partition.getNormal());
        tree->front->isLeaf = true;
        tree->front->isSolid = false;
    }

    if (tree->back == nullptr) {
        tree->back = new BSPTreeNode(vPosVal(partition, 0), partition.getNormal());
        tree->back->isLeaf = true;
        tree->back->isSolid = true;
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

// void AddPolygon(Chimera::VertexData* Vertices, int NOV, std::vector<Triangle>* PolygonList) {
//     int loop;
//     int v0, v1, v2;
//     int NumberOfIndices = (NOV - 2) * 3;
//     for (loop = 0; loop < NumberOfIndices / 3; loop++) {
//         if (loop == 0) {
//             v0 = 0;
//             v1 = 1;
//             v2 = 2;
//         } else {
//             v1 = v2;
//             v2++;
//         }

//         Triangle t = Triangle(Vertices[v0], Vertices[v1], Vertices[v2]);

//         // generate polygon normal
//         t.generateNormal();

//         PolygonList->push_back(t);
//     }
// }

// void initPolygons(unsigned char* map, std::vector<Triangle>* PolygonList) {
//     Chimera::VertexData vl[4][4];
//     int direction[4] = {0};
//     for (int y = 0; y < 40; y++) {
//         for (int x = 0; x < 20; x++) {
//             int offset = (y * 20) + x;
//             // check what the digit is in the current map location
//             if (map[offset] != 0) {
//                 if (map[offset] == 2) { // North East Wall
//                     vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                     vl[0][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                     vl[0][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                     vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                     direction[0] = 1;
//                 }
//                 if (map[offset] == 3) { // North West Wall
//                     vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                     vl[0][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                     vl[0][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                     vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                     direction[0] = 1;
//                 }

//                 if (map[offset] == 1) { // Its a Standared wall
//                     if (x > 0) {
//                         if (map[offset - 1] == 0) { // if theres nothing to the left add a left facingwall
//                             vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                             vl[0][1] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                             vl[0][2] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                             vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                             direction[0] = 1;
//                         }
//                     }
//                     if (x < 19) {
//                         if (map[offset + 1] == 0) { // if there is nothing to the right add a right facing wall
//                             vl[1][0] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                             vl[1][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                             vl[1][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                             vl[1][3] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                             direction[1] = 1;
//                         }
//                     }
//                     if (y > 0) {
//                         if (map[offset - 20] == 0) { // if there is nothing south add a south facing wall
//                             vl[2][0] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                             vl[2][1] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                             vl[2][2] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                             vl[2][3] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                             direction[2] = 1;
//                         }
//                     }
//                     if (y < 39) {
//                         if (map[offset + 20] == 0) { // if there is nothing to the north add a north facing wall
//                             vl[3][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                             vl[3][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                             vl[3][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                             vl[3][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                             direction[3] = 1;
//                         }
//                     }
//                 } // end for if offset==1

//                 // build the polygons
//                 for (int a = 0; a < 4; a++) {
//                     if (direction[a] != 0)
//                         AddPolygon(&vl[a][0], 4, PolygonList);
//                 }
//             } // end for if offset!=0
//         }
//     }
//     // BSPTreeRootNode = new NODE;
//     // BuildBspTree(BSPTreeRootNode, PolygonList);
// }
} // namespace Chimera
