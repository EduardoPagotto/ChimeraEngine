#pragma once
#include "chimera_core/gl/Texture.hpp"
#include "chimera_core/gl/TextureParams.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace ce {

    struct FrameBufferSpecification {
        FrameBufferSpecification() = default;
        uint32_t width = 800, height = 600;
        std::vector<TexParam> attachments;
    };

    class FrameBuffer {
      public:
        FrameBuffer(const FrameBufferSpecification& spec);
        ~FrameBuffer();

        void bind() const;
        static void unbind();
        void clear_attachment(uint32_t attachment_index, const int value);
        void resize(const uint32_t& width, const uint32_t& height);
        int read_pixel(uint32_t attachment_index, int x, int y);
        inline std::shared_ptr<Texture> get_color_attachemnt(uint32_t index) const { return color_attachments_[index]; }
        inline const uint32_t width() const { return spec_.width; }
        inline const uint32_t height() const { return spec_.height; }
        inline std::shared_ptr<Texture> get_depth_attachemnt() { return depth_attachment_; }
        // void clearDepth(const glm::vec4& value) const;

      private:
        void destroy();
        void invalidade();

        uint32_t fram_buffer_id_;
        uint32_t rbo_;
        FrameBufferSpecification spec_;
        std::shared_ptr<Texture> depth_attachment_;
        TexParam depth_tex_spec_;
        TexParam rbo_spec_;
        std::vector<std::shared_ptr<Texture>> color_attachments_;
        std::vector<TexParam> color_tex_specs_;
    };
} // namespace ce
