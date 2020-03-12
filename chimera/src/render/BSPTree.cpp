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

void splitTriangle(const glm::vec3& fx, Chimera::Triangle* _pTriangle, PlanePoint* hyperPlane,
                   std::vector<Chimera::Triangle>* _pListPolygon) {
    glm::vec3& a = _pTriangle->vertex[0].position;
    glm::vec3& b = _pTriangle->vertex[1].position;
    glm::vec3& c = _pTriangle->vertex[2].position;

    // acerto para vertex do tex final igualar a rotacao do triangulo
    Chimera::VertexData* pVertex_a = nullptr;
    Chimera::VertexData* pVertex_b = nullptr;
    Chimera::VertexData* pVertex_c = nullptr;

    // Normaliza Triangulo para que o corte do hiper-plano esteja nos segmentos de reta CA e CB (corte em a e b)
    if (fx.x * fx.z >= 0) { // corte em a e c
        swapFace(b, c);
        swapFace(a, b);

        pVertex_a = &_pTriangle->vertex[2]; // old c
        pVertex_b = &_pTriangle->vertex[0]; // old a
        pVertex_c = &_pTriangle->vertex[1]; // old b

    } else if (fx.y * fx.z >= 0) { // corte em b e c
        swapFace(a, c);
        swapFace(a, b);
        //--
        pVertex_a = &_pTriangle->vertex[1]; // old b
        pVertex_b = &_pTriangle->vertex[2]; // old c
        pVertex_c = &_pTriangle->vertex[0]; // old a

    } else { // Cortre em a e b

        pVertex_a = &_pTriangle->vertex[0]; // old a
        pVertex_b = &_pTriangle->vertex[1]; // old b
        pVertex_c = &_pTriangle->vertex[2]; // old c
    }

    float propAC = 0.0;
    float propBC = 0.0;
    glm::vec3 A, B;

    intersect(&a, &c, hyperPlane, &A, &propAC);
    intersect(&b, &c, hyperPlane, &B, &propBC);

    // PA texture coord
    glm::vec2 deltaA = (pVertex_c->texture - pVertex_a->texture) * propAC;
    glm::vec2 texA = pVertex_a->texture + deltaA;

    // PB texture coord
    glm::vec2 deltaB = (pVertex_c->texture - pVertex_b->texture) * propBC;
    glm::vec2 texB = pVertex_b->texture + deltaB;

    //-- T1
    Chimera::Triangle T1(a, b, A);
    T1.vertex[0].texture = pVertex_a->texture; // a old b
    T1.vertex[1].texture = pVertex_b->texture; // b old c
    T1.vertex[2].texture = texA;               // A

    //-- T2
    Chimera::Triangle T2(b, B, A);
    T2.vertex[0].texture = pVertex_b->texture; // b old c
    T2.vertex[1].texture = texB;               // B
    T2.vertex[2].texture = texA;               // A

    // -- T3
    Chimera::Triangle T3(A, B, c);
    T3.vertex[0].texture = texA;               // A
    T3.vertex[1].texture = texB;               // B
    T3.vertex[2].texture = pVertex_c->texture; // c old a

    for (int i = 0; i < 3; i++) {
        T1.vertex[i].normal = _pTriangle->vertex[i].normal;
        T2.vertex[i].normal = _pTriangle->vertex[i].normal;
        T3.vertex[i].normal = _pTriangle->vertex[i].normal;
    }

    _pListPolygon->push_back(T1);
    _pListPolygon->push_back(T2);
    _pListPolygon->push_back(T3);
}

BSPTreeNode* bsptreeBuild(std::vector<Chimera::Triangle>* _pListPolygon) {

    if (_pListPolygon->empty() == true)
        return nullptr;

    // balanceador
    unsigned int bether_index = selectBestSplitter(*_pListPolygon);
    Chimera::Triangle better = (*_pListPolygon)[bether_index];
    BSPTreeNode* tree = new BSPTreeNode(better);

    PlanePoint hyperPlane(&tree->partition);

    std::vector<Chimera::Triangle> front_list;
    std::vector<Chimera::Triangle> back_list;

    while (_pListPolygon->empty() == false) {

        Chimera::Triangle poly = _pListPolygon->back();
        _pListPolygon->pop_back();
        glm::vec3 result;
        SIDE teste = classifyPoly(&hyperPlane, &poly, &result);

        if (teste == SIDE::CP_BACK)
            back_list.push_back(poly);
        else if (teste == SIDE::CP_FRONT)
            front_list.push_back(poly);
        else if (teste == SIDE::CP_ONPLANE)
            tree->polygons.push_back(poly);
        else // CP_SPANNING
            splitTriangle(result, &poly, &hyperPlane, _pListPolygon);
    }

    tree->front = bsptreeBuild(&front_list);
    tree->back = bsptreeBuild(&back_list);

    // leaf sem poligonos apenas para saber se solido ou vazio
    if (tree->front == nullptr) {
        tree->front = new BSPTreeNode(tree->partition);
        tree->front->isLeaf = true;
        tree->front->isSolid = false;
    }

    if (tree->back == nullptr) {
        tree->back = new BSPTreeNode(tree->partition);
        tree->back->isLeaf = true;
        tree->back->isSolid = true;
    }

    return tree;
}

//------PARSER METODOS------

void drawPolygon(BSPTreeNode* tree, std::vector<Chimera::VertexData>* _pOutVertex, bool logdata, bool frontSide) {
    // tree->arrayTriangle.DrawPolygons(); // Abaixo equivale a esta linha
    for (auto it = tree->polygons.begin(); it != tree->polygons.end(); it++) {
        Chimera::Triangle t = (*it);

        // if (t.getSerial() == 10) // 8, 9, 10
        //     continue;

        _pOutVertex->push_back(t.vertex[0]);
        _pOutVertex->push_back(t.vertex[1]);
        _pOutVertex->push_back(t.vertex[2]);

        // FIXME: remover depois de concluir o algoritmo
        if (logdata == true) {
            if (frontSide == true)
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Face F: %d", t.getSerial());
            else
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Face B: %d", t.getSerial());
        }
    }
}

void traverseTree(BSPTreeNode* tree, glm::vec3* pos, std::vector<Chimera::VertexData>* _pOutVertex, bool logdata) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    // no de indicador de final/solido
    if (tree->isLeaf == true)
        return;

    PlanePoint hyperPlane(&tree->partition);

    SIDE result = classifyPoint(pos, &hyperPlane);
    if (result == SIDE::CP_FRONT) {

        traverseTree(tree->back, pos, _pOutVertex, logdata);
        drawPolygon(tree, _pOutVertex, logdata, true);
        traverseTree(tree->front, pos, _pOutVertex, logdata);

    } else if (result == SIDE::CP_BACK) {

        traverseTree(tree->front, pos, _pOutVertex, logdata);
        drawPolygon(tree, _pOutVertex, logdata, false);
        traverseTree(tree->back, pos, _pOutVertex, logdata);

    } else { // result == SIDE::CP_ONPLANE
        // the eye point is on the partition hyperPlane...
        traverseTree(tree->front, pos, _pOutVertex, logdata);
        traverseTree(tree->back, pos, _pOutVertex, logdata);
    }
}

bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree) {
    float temp;
    glm::vec3 intersection;
    if (tree->isLeaf == true) {
        return !tree->isSolid;
    }

    PlanePoint hyperPlane(&tree->partition);

    SIDE PointA = classifyPoint(Start, &hyperPlane);
    SIDE PointB = classifyPoint(End, &hyperPlane);

    if (PointA == SIDE::CP_ONPLANE && PointB == SIDE::CP_ONPLANE) {
        return lineOfSight(Start, End, tree->front);
    }

    if (PointA == SIDE::CP_FRONT && PointB == SIDE::CP_BACK) {
        intersect(Start, End, &hyperPlane, &intersection, &temp);
        return lineOfSight(Start, &intersection, tree->front) && lineOfSight(End, &intersection, tree->back);
    }

    if (PointA == SIDE::CP_BACK && PointB == SIDE::CP_FRONT) {
        intersect(Start, End, &hyperPlane, &intersection, &temp);
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

unsigned int selectBestSplitter(std::vector<Chimera::Triangle>& _poliyList) {

    if (_poliyList.size() == 0)
        return 0;

    unsigned int indice_splitter = 0;
    unsigned int selectedPoly = 0;
    unsigned int bestScore = 100000; // just set to a very high value to begin

    while (indice_splitter < _poliyList.size()) {

        // Chimera::Triangle splitter = _poliyList[indice_splitter];
        PlanePoint hyperPlane(&_poliyList[indice_splitter]);

        long long score, splits, backfaces, frontfaces;
        score = splits = backfaces = frontfaces = 0;

        unsigned int indice_current = 0;
        while (indice_current < _poliyList.size()) {

            Chimera::Triangle currentPoly = _poliyList[indice_current];

            if (indice_current != indice_splitter) {
                glm::vec3 temp;

                SIDE result = classifyPoly(&hyperPlane, &currentPoly, &temp);
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

            indice_current++;

        } // end while current poly

        score = abs(frontfaces - backfaces) + (splits * 8);

        if (score < bestScore) {
            bestScore = score;
            selectedPoly = indice_splitter;
        }

        indice_splitter++;
    } // end while splitter == null
    return selectedPoly;
}

void bsptreeDraw(BSPTreeNode* _pRoot, glm::vec3* pos, std::vector<Chimera::VertexData>* _pOutVertex, bool logdata) {
    traverseTree(_pRoot, pos, _pOutVertex, logdata);
}

void AddPolygon(Chimera::VertexData* Vertices, int NOV, std::vector<Chimera::Triangle>* PolygonList) {
    int loop;
    int v0, v1, v2;
    int NumberOfIndices = (NOV - 2) * 3;
    for (loop = 0; loop < NumberOfIndices / 3; loop++) {
        if (loop == 0) {
            v0 = 0;
            v1 = 1;
            v2 = 2;
        } else {
            v1 = v2;
            v2++;
        }

        Chimera::Triangle t = Chimera::Triangle(Vertices[v0], Vertices[v1], Vertices[v2]);

        // generate polygon normal
        t.generateNormal();

        PolygonList->push_back(t);
    }
}

void initPolygons(unsigned char* map, std::vector<Chimera::Triangle>* PolygonList) {
    Chimera::VertexData vl[4][4];
    int direction[4] = {0};
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 20; x++) {
            int offset = (y * 20) + x;
            // check what the digit is in the current map location
            if (map[offset] != 0) {
                if (map[offset] == 2) { // North East Wall
                    vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
                    vl[0][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
                    vl[0][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
                    vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
                    direction[0] = 1;
                }
                if (map[offset] == 3) { // North West Wall
                    vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
                    vl[0][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
                    vl[0][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
                    vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
                    direction[0] = 1;
                }

                if (map[offset] == 1) { // Its a Standared wall
                    if (x > 0) {
                        if (map[offset - 1] == 0) { // if theres nothing to the left add a left facingwall
                            vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
                            vl[0][1] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
                            vl[0][2] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
                            vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
                            direction[0] = 1;
                        }
                    }
                    if (x < 19) {
                        if (map[offset + 1] == 0) { // if there is nothing to the right add a right facing wall
                            vl[1][0] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
                            vl[1][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
                            vl[1][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
                            vl[1][3] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
                            direction[1] = 1;
                        }
                    }
                    if (y > 0) {
                        if (map[offset - 20] == 0) { // if there is nothing south add a south facing wall
                            vl[2][0] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
                            vl[2][1] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
                            vl[2][2] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
                            vl[2][3] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
                            direction[2] = 1;
                        }
                    }
                    if (y < 39) {
                        if (map[offset + 20] == 0) { // if there is nothing to the north add a north facing wall
                            vl[3][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
                            vl[3][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
                            vl[3][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
                            vl[3][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
                            direction[3] = 1;
                        }
                    }
                } // end for if offset==1

                // build the polygons
                for (int a = 0; a < 4; a++) {
                    if (direction[a] != 0)
                        AddPolygon(&vl[a][0], 4, PolygonList);
                }
            } // end for if offset!=0
        }
    }
    // BSPTreeRootNode = new NODE;
    // BuildBspTree(BSPTreeRootNode, PolygonList);
}
} // namespace Chimera
