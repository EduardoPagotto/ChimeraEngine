#include "chimera_core/partition/BSPTree.hpp"
#include <SDL3/SDL.h>

namespace ce {

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
    //                 if (alpha.classifyPoly(vertex[th2->idx.s].point, vertex[th2->idx.t].point,
    //                 vertex[th2->idx.p].point, &result) !=
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

    BSPTreeNode* BspTree::create(Mesh& mesh, std::vector<TrisIndex>& vp_leaf_out) {

        std::list<std::shared_ptr<Triangle>> vtris;

        meshToTriangle(mesh, vtris);

        vertex_.assign(mesh.vertex.begin(), mesh.vertex.end());

        // create BspTtree leafy
        BSPTreeNode* root = build(vtris);

        vp_leaf_out.assign(this->vp_leaf_.begin(), this->vp_leaf_.end());

        this->vp_leaf_.clear();

        mesh.vertex.clear();
        mesh.vertex.assign(vertex_.begin(), vertex_.end());
        return root;
    }

    std::shared_ptr<Triangle> BspTree::select_best_splitter(std::list<std::shared_ptr<Triangle>>& v_triangle) {

        std::shared_ptr<Triangle> selected_triangle = nullptr; // poit to none
        glm::vec3 temp;                                        // inutil
        int64_t best_score{100000}, score{0}, splits{0}, backfaces{0}, frontfaces{0};

        for (std::shared_ptr<Triangle> th : v_triangle) {

            if (th->splitter == true)
                continue;

            score = splits = backfaces = frontfaces = 0;

            Plane hyper_plane(vertex_[th->idx.s].point, th->normal);

            for (std::shared_ptr<Triangle> current_poly : v_triangle) {
                if (current_poly != th) {
                    SIDE result = hyper_plane.classify_poly(vertex_[current_poly->idx.s].point, // PA
                                                            vertex_[current_poly->idx.t].point, // PB
                                                            vertex_[current_poly->idx.p].point, // PC
                                                            temp); // Clip Test Result (A,B,C)
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

            score = std::abs(frontfaces - backfaces) + (splits * 8);

            if (score < best_score) {
                best_score = score;
                selected_triangle = th;
            }

        } // end while splitter

        if (selected_triangle != nullptr)
            selected_triangle->splitter = true;

        return selected_triangle;
    }

    void BspTree::split_triangle(const glm::vec3& fx, std::shared_ptr<Triangle> p_triangle, Plane& hyper_plane,
                                 std::list<std::shared_ptr<Triangle>>& v_triangle) {

        // Vertex dos triangulos a serem normalizados
        glm::vec2 vert_a_uv, vert_b_uv, vert_c_uv;

        glm::vec3 a{vertex_[p_triangle->idx.s].point};
        glm::vec3 b{vertex_[p_triangle->idx.t].point};
        glm::vec3 c{vertex_[p_triangle->idx.p].point};

        // Normaliza Triangulo para que o corte do triangulo esteja nos segmentos de reta CA e CB (corte em a e b)
        if (fx.x * fx.z >= 0) {                        // corte em a e c (rotaciona pontos sentido horario) ABC => BCA
            swapFace(b, c);                            // troca b com c
            swapFace(a, b);                            // troca a com b
            vert_a_uv = vertex_[p_triangle->idx.p].uv; // old c
            vert_b_uv = vertex_[p_triangle->idx.s].uv; // old a
            vert_c_uv = vertex_[p_triangle->idx.t].uv; // old b

        } else if (fx.y * fx.z >= 0) { // corte em b e c (totaciona pontos sentido anti-horario)  ABC => CAB
            swapFace(a, c);            // troca A com C
            swapFace(a, b);            // troca a com b
            vert_a_uv = vertex_[p_triangle->idx.t].uv; // old b
            vert_b_uv = vertex_[p_triangle->idx.p].uv; // old c
            vert_c_uv = vertex_[p_triangle->idx.s].uv; // old a

        } else {                                       // Cortre em a e b (pontos posicao original)
            vert_a_uv = vertex_[p_triangle->idx.s].uv; // old a
            vert_b_uv = vertex_[p_triangle->idx.t].uv; // old b
            vert_c_uv = vertex_[p_triangle->idx.p].uv; // old c
        }

        glm::vec3 ta, tb;       // Pega pontos posicao original e inteseccao
        float prop_ac, prop_bc; // Proporcao de textura (0.0 a 1.0)

        hyper_plane.intersect(a, c, ta, prop_ac);
        hyper_plane.intersect(b, c, tb, prop_bc);

        // PA texture coord
        const glm::vec2 delta_a{(vert_c_uv - vert_a_uv) * prop_ac};
        const glm::vec2 tex_a{vert_a_uv + delta_a};

        // PB texture coord
        const glm::vec2 delta_b{(vert_c_uv - vert_b_uv) * prop_bc};
        const glm::vec2 tex_b{vert_b_uv + delta_b};

        // indices de triangulos novos
        size_t last = vertex_.size();

        //-- T1 Triangle T1(a, b, A); // mesma normal que o original
        vertex_.push_back({a, p_triangle->normal, vert_a_uv}); // T1 PA
        vertex_.push_back({b, p_triangle->normal, vert_b_uv}); // T1 PB
        vertex_.push_back({ta, p_triangle->normal, tex_a});    // T1 PC
        v_triangle.push_front(
            std::make_shared<Triangle>(glm::uvec3(last, last + 1, last + 2), p_triangle->normal, p_triangle->splitter));

        //-- T2 Triangle T2(b, B, A); // mesma normal que o original
        vertex_.push_back({b, p_triangle->normal, vert_b_uv}); // T2 PA
        vertex_.push_back({tb, p_triangle->normal, tex_b});    // T2 PB
        vertex_.push_back({ta, p_triangle->normal, tex_a});    // T2 PC
        v_triangle.push_front(std::make_shared<Triangle>(glm::uvec3(last + 3, last + 4, last + 5), p_triangle->normal,
                                                         p_triangle->splitter));

        // -- T3 Triangle T3(A, B, c); // mesma normal que o original
        vertex_.push_back({ta, p_triangle->normal, tex_a});    // T3 PA
        vertex_.push_back({tb, p_triangle->normal, tex_b});    // T3 PB
        vertex_.push_back({c, p_triangle->normal, vert_c_uv}); // T3 PC
        v_triangle.push_front(std::make_shared<Triangle>(glm::uvec3(last + 6, last + 7, last + 8), p_triangle->normal,
                                                         p_triangle->splitter));

        // Remove orininal
        //_pTriangle.reset(); // Preciso disto aqui ???
        // int aa = _pTriangle.use_count();
    }

    BSPTreeNode* BspTree::build(std::list<std::shared_ptr<Triangle>>& v_triangle) {

        if (v_triangle.empty())
            return nullptr;

        std::list<std::shared_ptr<Triangle>> front_list;
        std::list<std::shared_ptr<Triangle>> back_list;

        std::shared_ptr<Triangle> poly = nullptr;
        BSPTreeNode* tree = nullptr;

        if (std::shared_ptr<Triangle> best = select_best_splitter(v_triangle); best != nullptr) {
            tree = new BSPTreeNode(Plane(vertex_[best->idx.s].point, best->normal));
            while (v_triangle.empty() == false) {

                poly = v_triangle.back();
                v_triangle.pop_back();
                glm::vec3 result;
                SIDE clip_test =
                    tree->hyperPlane.classify_poly(vertex_[poly->idx.s].point, // PA old poly.vertex[0].point
                                                   vertex_[poly->idx.t].point, // PB
                                                   vertex_[poly->idx.p].point, // PC
                                                   result);                    // Clip Test Result (A,B,C)
                switch (clip_test) {
                    case SIDE::CP_BACK:
                        back_list.push_front(poly);
                        break;
                    case SIDE::CP_FRONT:
                        front_list.push_front(poly);
                        break;
                    case SIDE::CP_ONPLANE: {
                        if (tree->hyperPlane.collinear_normal(poly->normal) == true)
                            front_list.push_front(poly);
                        else
                            back_list.push_front(poly);
                    } break;
                    default:
                        split_triangle(result, poly, tree->hyperPlane, v_triangle);
                        break;
                }
            }
        } else {
            // FIXME: used only to test broken vertexdata map
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Falha no BackFace");
            bool primeiro = true;
            while (v_triangle.empty() == false) {
                poly = v_triangle.back();
                v_triangle.pop_back();
                if (primeiro == true) {
                    tree = new BSPTreeNode(Plane(vertex_[poly->idx.s].point, poly->normal));
                    primeiro = false;
                }
                front_list.push_front(poly);
            }
        }

        size_t count = 0;
        for (auto th : front_list) {
            if (th->splitter == false)
                count++;
        }

        if (count == 0) {
            create_leafy(tree, front_list);
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

    void BspTree::create_leafy(BSPTreeNode* tree, std::list<std::shared_ptr<Triangle>>& list_convex_triangle) {

        TrisIndex leaf;
        while (list_convex_triangle.empty() == false) {
            std::shared_ptr<Triangle> conv_poly = list_convex_triangle.back();
            list_convex_triangle.pop_back();
            leaf.push_back(conv_poly->idx);

            // delete convPoly;
            // convPoly = nullptr;
        }

        tree->leafIndex = vp_leaf_.size();
        vp_leaf_.push_back(leaf);

        tree->isSolid = false;
        tree->isLeaf = true;
    }
} // namespace ce
