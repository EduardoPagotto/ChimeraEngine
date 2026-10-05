#pragma once
#include "IRenderer3d.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_core/visible/Mesh.hpp"
#include "chimera_space/AABB.hpp"
#include "chimera_space/BSPTreeNode.hpp"

namespace ce {

    class RenderableBsp : public Renderable3D {
      public:
        RenderableBsp(Mesh& mesh);

        virtual ~RenderableBsp();

        const uint32_t get_size() const override { return tot_index_; }

        std::shared_ptr<IndexBuffer> get_ibo() const override { return nullptr; }

        const AABB& get_aabb() const override { return aabb_; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

      private:
        void destroy();

        void collapse(BSPTreeNode* tree);

        void traverse_tree(const glm::vec3& camera_pos, BSPTreeNode* tree, std::vector<Renderable3D*>& child_draw);

        // TODO: Testar!!!!!!
        bool line_of_sight(const glm::vec3& start, const glm::vec3& end, BSPTreeNode* tree);

      private:
        std::vector<Renderable3D*> v_child_;
        AABB aabb_;
        uint32_t tot_index_;
        BSPTreeNode* root_;
    };
} // namespace ce
