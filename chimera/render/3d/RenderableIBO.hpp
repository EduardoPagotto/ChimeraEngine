#pragma once
#include "chimera/core/gl/buffer/IndexBuffer.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/space/AABB.hpp"

namespace ce {

    class RenderableIBO : public Renderable3D {

      private:
        std::shared_ptr<IndexBuffer> ibo;
        AABB aabb;

      public:
        RenderableIBO(std::shared_ptr<VertexArray> vao, std::shared_ptr<IndexBuffer> ibo, const AABB& aabb);

        virtual ~RenderableIBO();

        const uint32_t getSize() const override { return ibo->getSize(); }

        std::shared_ptr<IndexBuffer> getIBO() const override { return ibo; }

        const AABB& getAABB() const override { return aabb; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

        void draw(const bool& logData) override;
    };
} // namespace ce
