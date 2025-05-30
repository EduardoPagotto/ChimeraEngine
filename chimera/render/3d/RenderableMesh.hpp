#pragma once
#include "chimera/core/gl/buffer/IndexBuffer.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/space/AABB.hpp"

namespace ce {

    class RenderableMesh : public Renderable3D {

      private:
        uint32_t totIndex;
        Renderable3D* child;
        AABB aabb;

      public:
        RenderableMesh(Mesh* mesh);

        virtual ~RenderableMesh();

        const uint32_t getSize() const override { return totIndex; }

        std::shared_ptr<IndexBuffer> getIBO() const override { return nullptr; }

        const AABB& getAABB() const override { return aabb; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;
    };
} // namespace ce
