#pragma once
#include "FrameBuffer.hpp"
#include "VertexBuffer.hpp"
#include "chimera_core/gl/Shader.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"

namespace ce {

    class RenderBuffer {
      public:
        RenderBuffer(const uint32_t& pos_x, const uint32_t& pos_y, std::shared_ptr<FrameBuffer> fb,
                     std::shared_ptr<Shader> shader);

        virtual ~RenderBuffer() {
            frame_buffer_.reset();
            vbo_.reset();
        }

        void render();

        void bind();

        void unbind() { frame_buffer_->unbind(); }

        inline const uint32_t width() const { return frame_buffer_->width(); }

        inline const uint32_t height() const { return frame_buffer_->height(); }

        inline std::shared_ptr<FrameBuffer> get_fram_buffer() const { return frame_buffer_; }

      private:
        uint32_t pos_x_, pos_y_;
        std::shared_ptr<Shader> shader_;
        std::shared_ptr<VertexBuffer> vbo_;
        std::shared_ptr<VertexArray> vao_;
        std::shared_ptr<FrameBuffer> frame_buffer_;
    };
} // namespace ce
