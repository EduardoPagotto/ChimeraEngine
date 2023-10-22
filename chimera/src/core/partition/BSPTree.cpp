#include "chimera/core/partition/BSPTree.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

template <class T>
void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

// bool BspTree::tringleListIsConvex(std::vector<std::shared_ptr<Triangle>>& _vTriangle) {

//     if (_vTriangle.size() <= 1)
//         return false;

//     glm::vec3 result;
//     std::shared_ptr<Triangle> th1 = nullptr;
//     std::shared_ptr<Triangle> th2 = nullptr;
//     for (std::vector<std::shared_ptr<Triangle>>::iterator i = _vTriangle.begin(); i != _vTriangle.end(); i++) {

//         th1 = (*i);
//         for (std::vector<std::shared_ptr<Triangle>>::iterator j = i; j != _vTriangle.end(); j++) {

//             if (i == j)
//                 continue;

//             th2 = (*j);
//             float val = glm::dot(th1->normal, th2->normal); // DOT(U,V)
//             if (val > 0.0f) {                               // if not convex test if is coplanar
//                 Plane alpha(vertex[th1->idx.s].point, th1->normal);
//                 if (alpha.classifyPoly(vertex[th2->idx.s].point, vertex[th2->idx.t].point, vertex[th2->idx.p].point, &result) !=
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

BSPTreeNode* BspTree::create(Mesh& mesh, std::vector<TrisIndex>& vpLeafOut) {
    // std::list<std::shared_ptr<Triangle>> vTris;
    std::list<std::shared_ptr<Triangle>> vTris;

    meshToTriangle(mesh, vTris);

    vertex.assign(mesh.vertex.begin(), mesh.vertex.end());

    // create BspTtree leafy
    BSPTreeNode* root = build(vTris);

    vpLeafOut.assign(this->vpLeaf.begin(), this->vpLeaf.end());

    this->vpLeaf.clear();

    mesh.vertex.clear();
    mesh.vertex.assign(vertex.begin(), vertex.end());
    return root;
}

std::shared_ptr<Triangle> BspTree::selectBestSplitter(std::list<std::shared_ptr<Triangle>>& _vTriangle) {

    unsigned int bestScore = 100000;                      // just set to a very high value to begin
    std::shared_ptr<Triangle> selectedTriangle = nullptr; // poit to none
    glm::vec3 temp;                                       // inutil
    long long score, splits, backfaces, frontfaces;       // counts

    for (std::shared_ptr<Triangle> th : _vTriangle) {

        if (th->splitter == true)
            continue;

        score = splits = backfaces = frontfaces = 0;

        Plane hyperPlane(vertex[th->idx.s].point, th->normal);

        for (std::shared_ptr<Triangle> currentPoly : _vTriangle) {
            if (currentPoly != th) {
                SIDE result = hyperPlane.classifyPoly(vertex[currentPoly->idx.s].point, // PA
                                                      vertex[currentPoly->idx.t].point, // PB
                                                      vertex[currentPoly->idx.p].point, // PC
                                                      &temp);                           // Clip Test Result (A,B,C)
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

void BspTree::splitTriangle(const glm::vec3& fx, std::shared_ptr<Triangle> _pTriangle, Plane& hyperPlane,
                            std::list<std::shared_ptr<Triangle>>& _vTriangle) {

    // Proporcao de textura (0.0 a 1.0)
    float propAC = 0.0;
    float propBC = 0.0;

    // Vertex dos triangulos a serem normalizados
    glm::vec2 vertA_uv, vertB_uv, vertC_uv;

    // Pega pontos posicao original e inteseccao
    glm::vec3 A, B;
    glm::vec3 a = vertex[_pTriangle->idx.s].point;
    glm::vec3 b = vertex[_pTriangle->idx.t].point;
    glm::vec3 c = vertex[_pTriangle->idx.p].point;

    // Normaliza Triangulo para que o corte do triangulo esteja nos segmentos de reta CA e CB (corte em a e b)
    if (fx.x * fx.z >= 0) {                      // corte em a e c (rotaciona pontos sentido horario) ABC => BCA
        swapFace(b, c);                          // troca b com c
        swapFace(a, b);                          // troca a com b
        vertA_uv = vertex[_pTriangle->idx.p].uv; // old c
        vertB_uv = vertex[_pTriangle->idx.s].uv; // old a
        vertC_uv = vertex[_pTriangle->idx.t].uv; // old b

    } else if (fx.y * fx.z >= 0) {               // corte em b e c (totaciona pontos sentido anti-horario)  ABC => CAB
        swapFace(a, c);                          // troca A com C
        swapFace(a, b);                          // troca a com b
        vertA_uv = vertex[_pTriangle->idx.t].uv; // old b
        vertB_uv = vertex[_pTriangle->idx.p].uv; // old c
        vertC_uv = vertex[_pTriangle->idx.s].uv; // old a

    } else {                                     // Cortre em a e b (pontos posicao original)
        vertA_uv = vertex[_pTriangle->idx.s].uv; // old a
        vertB_uv = vertex[_pTriangle->idx.t].uv; // old b
        vertC_uv = vertex[_pTriangle->idx.p].uv; // old c
    }

    hyperPlane.intersect(a, c, A, propAC);
    hyperPlane.intersect(b, c, B, propBC);

    // PA texture coord
    glm::vec2 deltaA = (vertC_uv - vertA_uv) * propAC;
    glm::vec2 texA = vertA_uv + deltaA;

    // PB texture coord
    glm::vec2 deltaB = (vertC_uv - vertB_uv) * propBC;
    glm::vec2 texB = vertB_uv + deltaB;

    // indices de triangulos novos
    unsigned int last = vertex.size();

    //-- T1 Triangle T1(a, b, A); // mesma normal que o original
    vertex.push_back({a, _pTriangle->normal, vertA_uv}); // T1 PA
    vertex.push_back({b, _pTriangle->normal, vertB_uv}); // T1 PB
    vertex.push_back({A, _pTriangle->normal, texA});     // T1 PC
    _vTriangle.push_front(std::make_shared<Triangle>(glm::uvec3(last, last + 1, last + 2), _pTriangle->normal, _pTriangle->splitter));

    //-- T2 Triangle T2(b, B, A); // mesma normal que o original
    vertex.push_back({b, _pTriangle->normal, vertB_uv}); // T2 PA
    vertex.push_back({B, _pTriangle->normal, texB});     // T2 PB
    vertex.push_back({A, _pTriangle->normal, texA});     // T2 PC
    _vTriangle.push_front(std::make_shared<Triangle>(glm::uvec3(last + 3, last + 4, last + 5), _pTriangle->normal, _pTriangle->splitter));

    // -- T3 Triangle T3(A, B, c); // mesma normal que o original
    vertex.push_back({A, _pTriangle->normal, texA});     // T3 PA
    vertex.push_back({B, _pTriangle->normal, texB});     // T3 PB
    vertex.push_back({c, _pTriangle->normal, vertC_uv}); // T3 PC
    _vTriangle.push_front(std::make_shared<Triangle>(glm::uvec3(last + 6, last + 7, last + 8), _pTriangle->normal, _pTriangle->splitter));

    // Remove orininal
    //_pTriangle.reset(); // Preciso disto aqui ???
    // int aa = _pTriangle.use_count();
}

BSPTreeNode* BspTree::build(std::list<std::shared_ptr<Triangle>>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    std::list<std::shared_ptr<Triangle>> front_list;
    std::list<std::shared_ptr<Triangle>> back_list;

    std::shared_ptr<Triangle> poly = nullptr;
    BSPTreeNode* tree = nullptr;

    if (std::shared_ptr<Triangle> best = selectBestSplitter(_vTriangle); best != nullptr) {
        tree = new BSPTreeNode(Plane(vertex[best->idx.s].point, best->normal));
        while (_vTriangle.empty() == false) {

            poly = _vTriangle.back();
            _vTriangle.pop_back();
            glm::vec3 result;
            SIDE clipTest = tree->hyperPlane.classifyPoly(vertex[poly->idx.s].point, // PA old poly.vertex[0].point
                                                          vertex[poly->idx.t].point, // PB
                                                          vertex[poly->idx.p].point, // PC
                                                          &result);                  // Clip Test Result (A,B,C)
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
                tree = new BSPTreeNode(Plane(vertex[poly->idx.s].point, poly->normal));
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

void BspTree::createLeafy(BSPTreeNode* tree, std::list<std::shared_ptr<Triangle>>& listConvexTriangle) {

    TrisIndex leaf;
    while (listConvexTriangle.empty() == false) {
        std::shared_ptr<Triangle> convPoly = listConvexTriangle.back();
        listConvexTriangle.pop_back();
        leaf.push_back(convPoly->idx);

        // delete convPoly;
        // convPoly = nullptr;
    }

    tree->leafIndex = vpLeaf.size();
    vpLeaf.push_back(leaf);

    tree->isSolid = false;
    tree->isLeaf = true;
}
} // namespace Chimera
