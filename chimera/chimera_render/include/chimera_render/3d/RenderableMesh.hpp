#pragma once
#include "chimera_core/gl/buffer/IndexBuffer.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_core/visible/Mesh.hpp"
#include "chimera_render/3d/IRenderer3d.hpp"
#include "chimera_space/AABB.hpp"

namespace ce {

    class RenderableMesh : public Renderable3D {
      public:
        RenderableMesh(Mesh* mesh);

        virtual ~RenderableMesh();

        const uint32_t get_size() const override { return tot_index_; }

        std::shared_ptr<IndexBuffer> get_ibo() const override { return nullptr; }

        const AABB& get_aabb() const override { return aabb_; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

      private:
        uint32_t tot_index_;
        Renderable3D* child_;
        AABB aabb_;
    };
} // namespace ce
