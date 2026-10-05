#pragma once
#include "Renderable2D.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include "chimera_core/gl/buffer/IndexBuffer.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"

namespace ce {

    constexpr uint32_t renderer_max_sprites{60000};
    constexpr uint32_t renderer_sprite_size{4};
    constexpr uint32_t renderer_buffer_size{renderer_sprite_size * renderer_max_sprites};
    constexpr uint32_t renderer_indices_size{renderer_max_sprites * 6};
    constexpr uint32_t renderer_max_texture{32};

    struct VertexDataSimple {
        glm::vec3 point; // 3 * 4 = 12 (0 - 11)
        glm::vec2 uv;    // 2 * 4 = 8  (12 - 19)
        float tid;       // 1 * 4 = 4  (20 - 23)
        glm::vec4 color; // 4 * 4 = 16 (24 - 39)
    };

    class BatchRender2D : public IRenderer2D {

      public:
        BatchRender2D();

        virtual ~BatchRender2D();

        void init();

        virtual void begin(std::shared_ptr<Camera> camera) override;

        virtual void submit(IRenderable2D* renderable) override;

        virtual void end() override;

        virtual void flush() override;

        virtual void draw_string(std::shared_ptr<Font> font, const std::string& text, const glm::vec3& pos,
                                 const glm::vec4& color) override;

        inline virtual TransformationStack& get_stack() override { return stack_; };

        inline virtual void set_command_render(struct RenderCommand* command) override { render_comando_ = command; }

      private:
        float submit_texture(std::shared_ptr<Texture> texture);

        TransformationStack stack_;
        std::shared_ptr<IndexBuffer> ibo_;
        std::shared_ptr<VertexArray> p_vao_;
        std::shared_ptr<VertexBuffer> p_vbo_;
        GLsizei index_count_;
        VertexDataSimple* buffer_;
        RenderCommand* render_comando_;
        std::vector<std::shared_ptr<Texture>> textures_;
        std::shared_ptr<Camera> camera_;
    };
} // namespace ce
