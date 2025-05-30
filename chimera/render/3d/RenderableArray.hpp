#pragma once
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/space/AABB.hpp"

namespace ce {

    class RenderableArray : public Renderable3D {
      public:
        RenderableArray(std::vector<TrisIndex>& vPtrTrisIndex, Mesh* mesh);
        virtual ~RenderableArray();

        virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;
        virtual const uint32_t getSize() const override { return totIndex; }
        virtual IndexBuffer* getIBO() const override { return nullptr; }
        virtual const AABB& getAABB() const override { return aabb; }

      private:
        std::vector<IRenderable3d*> vChild;
        AABB aabb;
        uint32_t totIndex;
    };
} // namespace ce
