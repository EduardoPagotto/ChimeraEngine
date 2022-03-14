#include "chimera/render/partition/BSPTree.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

template <class T> void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

// bool tringleListIsConvex(std::vector<VertexData>& vertexList, std::vector<Triangle*>& _vTriangle) {

//     if (_vTriangle.size() <= 1)
//         return false;

//     glm::vec3 result;
//     Triangle* th1 = nullptr;
//     Triangle* th2 = nullptr;
//     for (std::vector<Triangle*>::iterator i = _vTriangle.begin(); i != _vTriangle.end(); i++) {

//         th1 = (*i);
//         for (std::vector<Triangle*>::iterator j = i; j != _vTriangle.end(); j++) {

//             if (i == j)
//                 continue;

//             th2 = (*j);
//             float val = glm::dot(th1->normal, th2->normal); // DOT(U,V)
//             if (val > 0.0f) {                               // if not convex test if is coplanar
//                 Plane alpha(vertexList[th1->p[TRI_PA]].position, th1->normal);
//                 if (alpha.classifyPoly(vertexList[th2->p[TRI_PA]].position, vertexList[th2->p[TRI_PB]].position,
//                                        vertexList[th2->p[TRI_PC]].position, &result) != SIDE::CP_ONPLANE)
//                     return false;

//                 // test if faces has oposites directions aka: convex
//                 if (alpha.collinearNormal(th1->normal) == false)
//                     return false;
//             }
//         }
//     }

//     return true;
// }

BSPTreeNode* BspTree::create(std::vector<VertexData>& _vVertexIn, std::list<Triangle*>& _vTriangleIn, std::vector<VertexData>& _vVertexOut,
                             VecPrtTrisIndex& vpLeafOut) {

    this->vVertex = std::move(_vVertexIn);

    // create BspTtree leafy
    BSPTreeNode* root = build(_vTriangleIn);

    _vVertexOut.assign(this->vVertex.begin(), this->vVertex.end());
    vpLeafOut.assign(this->vpLeaf.begin(), this->vpLeaf.end());

    this->vpLeaf.clear();
    this->vVertex.clear();

    return root;
}

Triangle* BspTree::selectBestSplitter(std::list<Triangle*>& _vTriangle) {

    unsigned int bestScore = 100000;                // just set to a very high value to begin
    Triangle* selectedTriangle = nullptr;           // poit to none
    glm::vec3 temp;                                 // inutil
    long long score, splits, backfaces, frontfaces; // counts

    for (Triangle* th : _vTriangle) {

        if (th->splitter == true)
            continue;

        score = splits = backfaces = frontfaces = 0;

        Plane hyperPlane(vVertex[th->p[TRI_PA]].position, th->normal);

        for (Triangle* currentPoly : _vTriangle) {
            if (currentPoly != th) {
                SIDE result = hyperPlane.classifyPoly(vVertex[currentPoly->p[TRI_PA]].position, // PA
                                                      vVertex[currentPoly->p[TRI_PB]].position, // PB
                                                      vVertex[currentPoly->p[TRI_PC]].position, // PC
                                                      &temp);                                   // Clip Test Result (A,B,C)
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
        selectedTriangle->splitter = true;

    return selectedTriangle;
}

void BspTree::splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle) {

    // Proporcao de textura (0.0 a 1.0)
    float propAC = 0.0;
    float propBC = 0.0;
    // indices de triangulos novos
    uint32_t p[9];
    for (uint8_t i = 0; i < 9; i++)
        p[i] = vVertex.size() + i;

    // Vertex dos triangulos a serem normalizados
    VertexData vertA, vertB, vertC;

    // Pega pontos posicao original e inteseccao
    glm::vec3 A, B;
    glm::vec3 a = vVertex[_pTriangle->p[TRI_PA]].position;
    glm::vec3 b = vVertex[_pTriangle->p[TRI_PB]].position;
    glm::vec3 c = vVertex[_pTriangle->p[TRI_PC]].position;

    // Normaliza Triangulo para que o corte do triangulo esteja nos segmentos de reta CA e CB (corte em a e b)
    if (fx.x * fx.z >= 0) {                     // corte em a e c (rotaciona pontos sentido horario) ABC => BCA
        swapFace(b, c);                         // troca b com c
        swapFace(a, b);                         // troca a com b
        vertA = vVertex[_pTriangle->p[TRI_PC]]; // old c
        vertB = vVertex[_pTriangle->p[TRI_PA]]; // old a
        vertC = vVertex[_pTriangle->p[TRI_PB]]; // old b

    } else if (fx.y * fx.z >= 0) {              // corte em b e c (totaciona pontos sentido anti-horario)  ABC => CAB
        swapFace(a, c);                         // troca A com C
        swapFace(a, b);                         // troca a com b
        vertA = vVertex[_pTriangle->p[TRI_PB]]; // old b
        vertB = vVertex[_pTriangle->p[TRI_PC]]; // old c
        vertC = vVertex[_pTriangle->p[TRI_PA]]; // old a

    } else {                                    // Cortre em a e b (pontos posicao original)
        vertA = vVertex[_pTriangle->p[TRI_PA]]; // old a
        vertB = vVertex[_pTriangle->p[TRI_PB]]; // old b
        vertC = vVertex[_pTriangle->p[TRI_PC]]; // old c
    }

    hyperPlane.intersect(a, c, &A, &propAC);
    hyperPlane.intersect(b, c, &B, &propBC);

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
    _vTriangle.push_front(new Triangle(p[0], p[1], p[2], normal, _pTriangle->splitter));

    //-- T2 Triangle T2(b, B, A);
    vVertex.push_back({b, vertB.normal, vertB.texture}); // T2 PA
    vVertex.push_back({B, vertB.normal, texB});          // T2 PB
    vVertex.push_back({A, vertA.normal, texA});          // T2 PC
    _vTriangle.push_front(new Triangle(p[3], p[4], p[5], normal, _pTriangle->splitter));

    // -- T3 Triangle T3(A, B, c);
    vVertex.push_back({A, vertA.normal, texA});          // T3 PA
    vVertex.push_back({B, vertB.normal, texB});          // T3 PB
    vVertex.push_back({c, vertC.normal, vertC.texture}); // T3 PC
    _vTriangle.push_front(new Triangle(p[6], p[7], p[8], normal, _pTriangle->splitter));

    // Remove orininal
    delete _pTriangle;
    _pTriangle = nullptr;
} // namespace Chimera

BSPTreeNode* BspTree::build(std::list<Triangle*>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    std::list<Triangle*> front_list;
    std::list<Triangle*> back_list;
    Triangle* poly = nullptr;
    BSPTreeNode* tree = nullptr;

    Triangle* best = selectBestSplitter(_vTriangle);
    if (best != nullptr) {
        tree = new BSPTreeNode(Plane(vVertex[best->p[TRI_PA]].position, best->normal));
        while (_vTriangle.empty() == false) {

            poly = _vTriangle.back();
            _vTriangle.pop_back();
            glm::vec3 result;
            SIDE clipTest = tree->hyperPlane.classifyPoly(vVertex[poly->p[TRI_PA]].position, // PA old poly.vertex[0].position
                                                          vVertex[poly->p[TRI_PB]].position, // PB
                                                          vVertex[poly->p[TRI_PC]].position, // PC
                                                          &result);                          // Clip Test Result (A,B,C)
            switch (clipTest) {
                case SIDE::CP_BACK:
                    back_list.push_front(poly);
                    break;
                case SIDE::CP_FRONT:
                    front_list.push_front(poly);
                    break;
                case SIDE::CP_ONPLANE: {
                    if (tree->hyperPlane.collinearNormal(poly->normal) == true)
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
                tree = new BSPTreeNode(Plane(vVertex[poly->p[TRI_PA]].position, poly->normal));
                primeiro = false;
            }
            front_list.push_front(poly);
        }
    }

    unsigned int count = 0;
    for (auto th : front_list) {
        if (th->splitter == false)
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

    TrisIndex* pLeaf = new TrisIndex(); // Renderable3D* pLeaf = new Renderable3D();

    while (listConvexTriangle.empty() == false) {
        Triangle* convPoly = listConvexTriangle.back();
        listConvexTriangle.pop_back();
        pLeaf->add(convPoly->p[TRI_PA], convPoly->p[TRI_PB], convPoly->p[TRI_PC]);

        delete convPoly;
        convPoly = nullptr;
    }

    tree->leafIndex = vpLeaf.size();
    vpLeaf.push_back(pLeaf);

    tree->isSolid = false;
    tree->isLeaf = true;
}
} // namespace Chimera
