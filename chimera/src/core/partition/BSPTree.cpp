#include "chimera/core/partition/BSPTree.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

template <class T>
void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

// bool BspTree::tringleListIsConvex(std::vector<Triangle*>& _vTriangle) {

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
//                 Plane alpha(mesh->point[th1->point.s], th1->normal);
//                 if (alpha.classifyPoly(mesh->point[th2->point.s], mesh->point[th2->point.t], mesh->point[th2->point.p], &result) !=
//                     SIDE::CP_ONPLANE)
//                     return false;

//                 // test if faces has oposites directions aka: convex
//                 if (alpha.collinearNormal(th1->normal) == false)
//                     return false;
//             }
//         }
//     }

//     return true;
// }

BSPTreeNode* BspTree::create(Mesh* mesh, std::vector<TrisIndex>& vpLeafOut) {
    std::list<Triangle*> vTris;
    meshToTriangle(mesh, vTris);

    this->mesh = mesh;

    // create BspTtree leafy
    BSPTreeNode* root = build(vTris);

    vpLeafOut.assign(this->vpLeaf.begin(), this->vpLeaf.end());

    this->vpLeaf.clear();

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

        Plane hyperPlane(mesh->point[th->point.s], th->normal);

        for (Triangle* currentPoly : _vTriangle) {
            if (currentPoly != th) {
                SIDE result = hyperPlane.classifyPoly(mesh->point[currentPoly->point.s], // PA
                                                      mesh->point[currentPoly->point.t], // PB
                                                      mesh->point[currentPoly->point.p], // PC
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
        selectedTriangle->splitter = true;

    return selectedTriangle;
}

void BspTree::addVertexMesh(const glm::vec3& point, const glm::vec3& normal, const glm::vec2& uv) {
    mesh->point.push_back(point);
    mesh->normal.push_back(normal);
    mesh->uv.push_back(uv);
}

void BspTree::splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle) {

    // Proporcao de textura (0.0 a 1.0)
    float propAC = 0.0;
    float propBC = 0.0;

    // Vertex dos triangulos a serem normalizados
    glm::vec2 vertA_uv, vertB_uv, vertC_uv;
    glm::vec3 vertA_normal, vertB_normal, vertC_normal;

    // Pega pontos posicao original e inteseccao
    glm::vec3 A, B;
    glm::vec3 a = mesh->point[_pTriangle->point.s];
    glm::vec3 b = mesh->point[_pTriangle->point.t];
    glm::vec3 c = mesh->point[_pTriangle->point.p];

    // Normaliza Triangulo para que o corte do triangulo esteja nos segmentos de reta CA e CB (corte em a e b)
    if (fx.x * fx.z >= 0) {                               // corte em a e c (rotaciona pontos sentido horario) ABC => BCA
        swapFace(b, c);                                   // troca b com c
        swapFace(a, b);                                   // troca a com b
        vertA_uv = mesh->uv[_pTriangle->point.p];         // old c
        vertB_uv = mesh->uv[_pTriangle->point.s];         // old a
        vertC_uv = mesh->uv[_pTriangle->point.t];         // old b
        vertA_normal = mesh->normal[_pTriangle->point.p]; // old c
        vertB_normal = mesh->normal[_pTriangle->point.s]; // old a
        vertC_normal = mesh->normal[_pTriangle->point.t]; // old b

    } else if (fx.y * fx.z >= 0) {                        // corte em b e c (totaciona pontos sentido anti-horario)  ABC => CAB
        swapFace(a, c);                                   // troca A com C
        swapFace(a, b);                                   // troca a com b
        vertA_uv = mesh->uv[_pTriangle->point.t];         // old b
        vertB_uv = mesh->uv[_pTriangle->point.p];         // old c
        vertC_uv = mesh->uv[_pTriangle->point.s];         // old a
        vertA_normal = mesh->normal[_pTriangle->point.t]; // old b
        vertB_normal = mesh->normal[_pTriangle->point.p]; // old c
        vertC_normal = mesh->normal[_pTriangle->point.s]; // old a

    } else {                                              // Cortre em a e b (pontos posicao original)
        vertA_uv = mesh->uv[_pTriangle->point.s];         // old a
        vertB_uv = mesh->uv[_pTriangle->point.t];         // old b
        vertC_uv = mesh->uv[_pTriangle->point.p];         // old c
        vertA_normal = mesh->normal[_pTriangle->point.s]; // old a
        vertB_normal = mesh->normal[_pTriangle->point.t]; // old b
        vertC_normal = mesh->normal[_pTriangle->point.p]; // old c
    }

    hyperPlane.intersect(a, c, A, propAC);
    hyperPlane.intersect(b, c, B, propBC);

    // PA texture coord
    glm::vec2 deltaA = (vertC_uv - vertA_uv) * propAC;
    glm::vec2 texA = vertA_uv + deltaA;

    // PB texture coord
    glm::vec2 deltaB = (vertC_uv - vertB_uv) * propBC;
    glm::vec2 texB = vertB_uv + deltaB;

    // Calcula Normal Face
    glm::vec3 acc = vertA_normal + vertB_normal + vertC_normal;
    glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);

    // indices de triangulos novos
    unsigned int last = mesh->point.size();

    //-- T1 Triangle T1(a, b, A);
    addVertexMesh(a, vertA_normal, vertA_uv); // T1 PA
    addVertexMesh(b, vertB_normal, vertB_uv); // T1 PB
    addVertexMesh(A, vertA_normal, texA);     // T1 PC
    _vTriangle.push_front(new Triangle(glm::uvec3(last, last + 1, last + 2), normal, _pTriangle->splitter));

    //-- T2 Triangle T2(b, B, A);
    addVertexMesh(b, vertB_normal, vertB_uv); // T2 PA
    addVertexMesh(B, vertB_normal, texB);     // T2 PB
    addVertexMesh(A, vertA_normal, texA);     // T2 PC
    _vTriangle.push_front(new Triangle(glm::uvec3(last + 3, last + 4, last + 5), normal, _pTriangle->splitter));

    // -- T3 Triangle T3(A, B, c);
    addVertexMesh(A, vertA_normal, texA);     // T3 PA
    addVertexMesh(B, vertB_normal, texB);     // T3 PB
    addVertexMesh(c, vertC_normal, vertC_uv); // T3 PC
    _vTriangle.push_front(new Triangle(glm::uvec3(last + 6, last + 7, last + 8), normal, _pTriangle->splitter));

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
        tree = new BSPTreeNode(Plane(mesh->point[best->point.s], best->normal));
        while (_vTriangle.empty() == false) {

            poly = _vTriangle.back();
            _vTriangle.pop_back();
            glm::vec3 result;
            SIDE clipTest = tree->hyperPlane.classifyPoly(mesh->point[poly->point.s], // PA old poly.vertex[0].point
                                                          mesh->point[poly->point.t], // PB
                                                          mesh->point[poly->point.p], // PC
                                                          &result);                   // Clip Test Result (A,B,C)
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
                tree = new BSPTreeNode(Plane(mesh->point[poly->point.s], poly->normal));
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

    TrisIndex leaf;
    while (listConvexTriangle.empty() == false) {
        Triangle* convPoly = listConvexTriangle.back();
        listConvexTriangle.pop_back();
        leaf.push_back(convPoly->point);

        delete convPoly;
        convPoly = nullptr;
    }

    tree->leafIndex = vpLeaf.size();
    vpLeaf.push_back(leaf);

    tree->isSolid = false;
    tree->isLeaf = true;
}
} // namespace Chimera
