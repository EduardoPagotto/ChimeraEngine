#pragma once
#include "chimera_core/gl/buffer/IndexBuffer.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_render/3d/IRenderer3d.hpp"
#include "chimera_space/AABB.hpp"

namespace ce {

    class RenderableIBO : public Renderable3D {
      public:
        RenderableIBO(std::shared_ptr<VertexArray> vao, std::shared_ptr<IndexBuffer> ibo, const AABB& aabb);

        virtual ~RenderableIBO();

        const uint32_t get_size() const override { return ibo_->get_size(); }

        std::shared_ptr<IndexBuffer> get_ibo() const override { return ibo_; }

        const AABB& get_aabb() const override { return aabb_; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

        void draw(const bool& logData) override;

      private:
        std::shared_ptr<IndexBuffer> ibo_;
        AABB aabb_;
    };
} // namespace ce
