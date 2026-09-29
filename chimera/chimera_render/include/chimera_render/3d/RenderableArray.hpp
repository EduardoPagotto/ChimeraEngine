#pragma once
#include "IRenderer3d.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_core/visible/Mesh.hpp"
#include "chimera_space/AABB.hpp"

namespace ce {

    class RenderableArray : public Renderable3D {
      public:
        RenderableArray(std::vector<TrisIndex>& vPtrTrisIndex, Mesh* mesh);

        virtual ~RenderableArray();

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

        const uint32_t getSize() const override { return tot_index_; }

        std::shared_ptr<IndexBuffer> getIBO() const override { return nullptr; }

        const AABB& getAABB() const override { return aabb_; }

      private:
        std::vector<Renderable3D*> v_child_;
        AABB aabb_;
        uint32_t tot_index_;
    };
} // namespace ce
