#include "chimera_render/3d/RenderableBsp.hpp"
#include "chimera_core/partition/BSPTree.hpp"
#include "chimera_render/3d/IRenderer3d.hpp"
#include "chimera_render/3d/RenderableIBO.hpp"

namespace ce {

    RenderableBsp::RenderableBsp(Mesh& mesh) : Renderable3D(), tot_index_(0) {

        Mesh mesh_final;
        mesh_reindex(mesh, mesh_final);

        BspTree bsp_tree;
        std::vector<TrisIndex> v_tris;
        root_ = bsp_tree.create(mesh_final, v_tris);

        // create VAO and VBO
        vao = std::make_shared<VertexArray>();
        vao->bind();

        std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>(BufferType::STATIC);
        vbo->bind();

        BufferLayout layout;
        layout.push<float>(3, false);
        layout.push<float>(3, false);
        layout.push<float>(2, false);

        vbo->set_layout(layout);
        vbo->set_data(&mesh_final.vertex[0], mesh_final.vertex.size());
        vbo->unbind();

        vao->push(vbo);

        // Add all leafs and create IBO
        for (auto tris_index : v_tris) {

            auto [min, max, size] = vertex_indexed_boundaries(mesh_final.vertex, tris_index);

            std::shared_ptr<IndexBuffer> ibo =
                std::make_shared<IndexBuffer>((uint32_t*)&tris_index[0], tris_index.size() * 3);

            Renderable3D* r = new RenderableIBO(vao, ibo, AABB(min, max));
            v_child_.push_back(r);
        }

        vao->unbind();

        auto [min, max, size] = vertex_boundaries(mesh_final.vertex);
        aabb_.set_boundary(min, max);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Childs: %ld", this->v_child_.size());
    }

    RenderableBsp::~RenderableBsp() { this->destroy(); }

    void RenderableBsp::traverse_tree(const glm::vec3& camera_pos, BSPTreeNode* tree,
                                      std::vector<Renderable3D*>& child_draw) {
        // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
        if ((tree != nullptr) && (tree->isSolid == false)) {
            switch (SIDE result = tree->hyperPlane.classify_point(camera_pos); result) {
                case SIDE::CP_FRONT: {
                    traverse_tree(camera_pos, tree->back, child_draw);
                    if (tree->isLeaf == true) // set to draw Polygon
                        child_draw.push_back(v_child_[tree->leafIndex]);

                    traverse_tree(camera_pos, tree->front, child_draw);
                } break;
                case SIDE::CP_BACK: {
                    traverse_tree(camera_pos, tree->front, child_draw);
                    if (tree->isLeaf == true) // set to draw Polygon
                        child_draw.push_back(v_child_[tree->leafIndex]);

                    traverse_tree(camera_pos, tree->back, child_draw);
                } break;
                default: { // SIDE::CP_ONPLANE  // the eye point is on the partition hyperPlane...
                    traverse_tree(camera_pos, tree->front, child_draw);
                    traverse_tree(camera_pos, tree->back, child_draw);
                } break;
            }
        }
    }

    void RenderableBsp::submit(RenderCommand& command, IRenderer3d& renderer) {
        std::vector<Renderable3D*> child_draw;
        const glm::vec3 camera_pos = renderer.get_camera()->get_position();
        traverse_tree(camera_pos, root_, child_draw);
        for (uint32_t c = 0; c < child_draw.size(); c++)
            renderer.submit(command, child_draw[c], c);

        child_draw.clear();
    }

    void RenderableBsp::destroy() {

        while (!v_child_.empty()) {
            Renderable3D* child = v_child_.back();
            v_child_.pop_back();
            delete child;
            child = nullptr;
        }

        collapse(root_);
    }

    void RenderableBsp::collapse(BSPTreeNode* tree) {

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

    bool RenderableBsp::line_of_sight(const glm::vec3& start, const glm::vec3& end, BSPTreeNode* tree) {
        float temp;
        glm::vec3 intersection;
        if (tree->isLeaf == true) {
            return !tree->isSolid;
        }

        const SIDE point_a = tree->hyperPlane.classify_point(start);
        const SIDE point_b = tree->hyperPlane.classify_point(end);

        if ((point_a == SIDE::CP_ONPLANE) && (point_b == SIDE::CP_ONPLANE)) {
            return line_of_sight(start, end, tree->front);
        }

        if ((point_a == SIDE::CP_FRONT) && (point_b == SIDE::CP_BACK)) {
            tree->hyperPlane.intersect(start, end, intersection, temp);
            return line_of_sight(start, intersection, tree->front) && line_of_sight(end, intersection, tree->back);
        }

        if ((point_a == SIDE::CP_BACK) && (point_b == SIDE::CP_FRONT)) {
            tree->hyperPlane.intersect(start, end, intersection, temp);
            return line_of_sight(end, intersection, tree->front) && line_of_sight(start, intersection, tree->back);
        }

        // if we get here one of the points is on the hyperPlane
        if ((point_a == SIDE::CP_FRONT) || (point_b == SIDE::CP_FRONT)) {
            return line_of_sight(start, end, tree->front);
        } else {
            return line_of_sight(start, end, tree->back);
        }
        return true;
    }
} // namespace ce
