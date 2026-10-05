#pragma once
#include "IRenderer3d.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_core/visible/Mesh.hpp"
#include "chimera_space/AABB.hpp"

namespace ce {

    class RenderableArray : public Renderable3D {
      public:
        RenderableArray(std::vector<TrisIndex>& v_ptr_tris_index, Mesh* mesh);

        virtual ~RenderableArray();

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

        const uint32_t get_size() const override { return tot_index_; }

        std::shared_ptr<IndexBuffer> get_ibo() const override { return nullptr; }

        const AABB& get_aabb() const override { return aabb_; }

      private:
        std::vector<Renderable3D*> v_child_;
        AABB aabb_;
        uint32_t tot_index_;
    };
} // namespace ce
