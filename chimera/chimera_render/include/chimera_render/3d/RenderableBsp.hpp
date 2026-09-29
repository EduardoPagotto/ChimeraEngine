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

        const uint32_t getSize() const override { return tot_index_; }

        std::shared_ptr<IndexBuffer> getIBO() const override { return nullptr; }

        const AABB& getAABB() const override { return aabb_; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

      private:
        void destroy();

        void collapse(BSPTreeNode* tree);

        void traverseTree(const glm::vec3& cameraPos, BSPTreeNode* tree, std::vector<Renderable3D*>& childDraw);

        // TODO: Testar!!!!!!
        bool lineOfSight(const glm::vec3& Start, const glm::vec3& End, BSPTreeNode* tree);

      private:
        std::vector<Renderable3D*> v_child_;
        AABB aabb_;
        uint32_t tot_index_;
        BSPTreeNode* root_;
    };
} // namespace ce
