#include "chimera/core/space/BSPTree.hpp"
#include <SDL2/SDL.h>

namespace Chimera::Core {

template <class T> void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

// RenderableBsp* BspTree::create(std::vector<Chimera::VertexData>& _vVertex, std::vector<uint32_t>& _vIndex) {
// void BspTree::create(std::vector<Chimera::VertexData>& vVertex, std::list<Triangle*>& vTris) {
void BspTree::create(std::vector<Chimera::VertexData>& _vVertex, std::vector<uint32_t>& _vIndex) {

    std::list<Triangle*> vTris;
    if (_vIndex.size() > 0) {
        triangleFromVertexDataIndex(&_vVertex[0], &_vIndex[0], _vIndex.size(), vTris);
    } else {
        triangleFromVertexData(&_vVertex[0], _vVertex.size(), vTris);
    }

    this->vVertex = std::move(_vVertex);

    // create BspTtree leafy
    root = build(vTris);

    _vIndex.clear();

    // vTris.clear();

    // RenderableBsp* r = new RenderableBsp(root, this->vpLeaf, this->vVertex);

    // return root;

    // return r;
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

BSPTreeNode* BspTree::build(std::list<Triangle*>& _vTriangle) {

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
        tree->front = build(front_list);
    }

    tree->back = build(back_list);
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

    RenderableIBO* pLeaf = new RenderableIBO();

    while (listConvexTriangle.empty() == false) {
        Triangle* convPoly = listConvexTriangle.back();
        listConvexTriangle.pop_back();
        pLeaf->addFace(convPoly->p[0], convPoly->p[1], convPoly->p[2]);

        delete convPoly;
        convPoly = nullptr;
    }

    tree->leafIndex = vpLeaf.size();
    vpLeaf.push_back(pLeaf);

    tree->isSolid = false;
    tree->isLeaf = true;
}
} // namespace Chimera::Core
