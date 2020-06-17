#include "chimera/render/BSPTree.hpp"
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

void BspTree::createSequencial(std::vector<Chimera::VertexData>& _vVertex) {

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

void BspTree::createIndexed(std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex) {

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

void BspTree::drawPolygon(BSPTreeNode* tree, bool frontSide) {

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

void BspTree::destroy() { collapse(root); }

void BspTree::collapse(BSPTreeNode* tree) {

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

Plane BspTree::selectBestSplitter(std::vector<Triangle*>& _vTriangle) {

    if (_vTriangle.size() == 0)
        return Plane();

    unsigned int selectedPoly = 0;
    unsigned int bestScore = 100000; // just set to a very high value to begin
    glm::vec3 temp;                  // inutil

    for (unsigned indice_splitter = 0; indice_splitter < _vTriangle.size(); indice_splitter++) {

        Triangle* th = _vTriangle[indice_splitter];
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

void BspTree::splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::vector<Triangle*>& _vTriangle) {

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
    _vTriangle.push_back(new Triangle(last++, last++, last++, normal));

    // //-- T2 Triangle T2(b, B, A);
    vVertex.push_back({b, vertB.normal, vertB.texture}); // T2 PA
    vVertex.push_back({B, vertB.normal, texB});          // T2 PB
    vVertex.push_back({A, vertA.normal, texA});          // T2 PC
    _vTriangle.push_back(new Triangle(last++, last++, last++, normal));

    // // -- T3 Triangle T3(A, B, c);
    vVertex.push_back({A, vertA.normal, texA});          // T3 PA
    vVertex.push_back({B, vertB.normal, texB});          // T3 PB
    vVertex.push_back({c, vertC.normal, vertC.texture}); // T3 PC
    _vTriangle.push_back(new Triangle(last++, last++, last++, normal));
}

bool BspTree::isConvex(std::vector<Triangle*>& _vTriangle, Triangle* _poly) {

    if (_vTriangle.size() <= 1)
        return false;

    Triangle* th1 = nullptr;
    Triangle* th2 = nullptr;
    float sameDir = 0.0;

    for (unsigned i = 0; i < _vTriangle.size(); i++) {

        th1 = _vTriangle[i];

        for (unsigned j = i; j < _vTriangle.size(); j++) {

            th2 = (i != j) ? _vTriangle[j] : _poly; // Test hyperplane is Convex too in coincident index

            glm::vec3 u = th1->getNormal();
            glm::vec3 v = th2->getNormal();
            float val = glm::dot(u, v);
            if (val > 0.0f) { // if not convex test if is coplanar
                glm::vec3 result;
                Plane alpha(vPosVal(th1, 0), th1->getNormal());
                SIDE clipTest = alpha.classifyPoly(vPosVal(th2, 0), vPosVal(th2, 1), vPosVal(th2, 2), &result);
                if (clipTest != SIDE::CP_ONPLANE)
                    return false;

                // test if faces has oposites directions
                glm::vec3 a = alpha.getNormal();
                glm::vec3 b = th1->getNormal();
                glm::vec3 sub = a - b;
                sameDir = (float)fabs(sub.x + sub.y + sub.z);
                if (sameDir >= EPSILON) // maior e oposto
                    return false;
            }
        }
    }

    return true;
}

BSPTreeNode* BspTree::bsptreeBuild(std::vector<Triangle*>& _vTriangle) {

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
            case SIDE::CP_ONPLANE:
                tree->polygons.push_back(poly);
                break;
            default:
                splitTriangle(result, poly, tree->hyperPlane, _vTriangle);
                break;
        }
    }

    // Verify if all triangles front are convex
    if (isConvex(front_list, poly) == true) {

        // next front only have conves set
        BSPTreeNode* convex = new BSPTreeNode(partition);
        while (front_list.empty() == false) {
            Triangle* convPoly = front_list.back();
            front_list.pop_back();
            convex->polygons.push_back(convPoly);
        }

        // ajust leaf and solid
        convex->front = new BSPTreeNode(partition);
        convex->front->isLeaf = true;
        convex->front->isSolid = false;
        convex->back = new BSPTreeNode(partition);
        convex->back->isLeaf = true;
        convex->back->isSolid = true;

        tree->front = convex;
    } else {
        tree->front = bsptreeBuild(front_list);
    }

    tree->back = bsptreeBuild(back_list);

    // leaf sem poligonos apenas para saber se solido ou vazio
    if (tree->front == nullptr) {
        tree->front = new BSPTreeNode(partition);
        tree->front->isLeaf = true;
        tree->front->isSolid = false;
    }

    if (tree->back == nullptr) {
        tree->back = new BSPTreeNode(partition);
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
} // namespace Chimera
