#pragma once
#include "FrameBuffer.hpp"
#include "VertexBuffer.hpp"
#include "chimera/core/gl/Shader.hpp"

namespace ce {

    class RenderBuffer {
      private:
        uint32_t posX, posY;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexBuffer> vbo;
        std::shared_ptr<FrameBuffer> frameBuffer;

      public:
        RenderBuffer(const uint32_t& posX, const uint32_t& posY, std::shared_ptr<FrameBuffer> fb,
                     std::shared_ptr<Shader> shader);

        virtual ~RenderBuffer() {
            frameBuffer.reset();
            vbo.reset();
        }

        void render();

        void bind();

        void unbind() { frameBuffer->unbind(); }

        inline const uint32_t getWidth() const { return frameBuffer->getWidth(); }

        inline const uint32_t getHeight() const { return frameBuffer->getHeight(); }

        inline std::shared_ptr<FrameBuffer> getFramBuffer() const { return frameBuffer; }
    };
} // namespace ce
