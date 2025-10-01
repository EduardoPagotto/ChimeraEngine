#pragma once
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/space/AABB.hpp"
#include "chimera/space/BSPTreeNode.hpp"

namespace ce {

    class RenderableBsp : public Renderable3D {

      private:
        std::vector<Renderable3D*> vChild;
        AABB aabb;
        uint32_t totIndex;
        BSPTreeNode* root;

      public:
        RenderableBsp(Mesh& mesh);

        virtual ~RenderableBsp();

        const uint32_t getSize() const override { return totIndex; }

        std::shared_ptr<IndexBuffer> getIBO() const override { return nullptr; }

        const AABB& getAABB() const override { return aabb; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

      private:
        void destroy();

        void collapse(BSPTreeNode* tree);

        void traverseTree(const glm::vec3& cameraPos, BSPTreeNode* tree, std::vector<Renderable3D*>& childDraw);

        // TODO: Testar!!!!!!
        bool lineOfSight(const glm::vec3& Start, const glm::vec3& End, BSPTreeNode* tree);
    };
} // namespace ce
