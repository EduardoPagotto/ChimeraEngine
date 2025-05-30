#pragma once
#include "Renderable2D.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/gl/buffer/IndexBuffer.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include <deque>

namespace ce {

#define RENDERER_MAX_SPRITES  60000
#define RENDERER_SPRITE_SIZE  4
#define RENDERER_BUFFER_SIZE  RENDERER_SPRITE_SIZE* RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURE  32

    struct VertexDataSimple {
        glm::vec3 point; // 3 * 4 = 12 (0 - 11)
        glm::vec2 uv;    // 2 * 4 = 8  (12 - 19)
        float tid;       // 1 * 4 = 4  (20 - 23)
        glm::vec4 color; // 4 * 4 = 16 (24 - 39)
    };

    class BatchRender2D : public IRenderer2D {

      private:
        TransformationStack stack;
        std::shared_ptr<IndexBuffer> ibo;
        std::shared_ptr<VertexArray> pVao;
        std::shared_ptr<VertexBuffer> pVbo;
        GLsizei indexCount;
        VertexDataSimple* buffer;
        RenderCommand* renderComando;
        std::vector<std::shared_ptr<Texture>> textures;
        std::shared_ptr<Camera> camera;

      public:
        BatchRender2D();

        virtual ~BatchRender2D();

        void init();

        virtual void begin(std::shared_ptr<Camera> camera) override;

        virtual void submit(IRenderable2D* renderable) override;

        virtual void end() override;

        virtual void flush() override;

        virtual void drawString(std::shared_ptr<Font> font, const std::string& text, const glm::vec3& pos,
                                const glm::vec4& color) override;

        inline virtual TransformationStack& getStack() override { return stack; };

        inline virtual void setCommandRender(struct RenderCommand* command) override { renderComando = command; }

      private:
        float submitTexture(std::shared_ptr<Texture> texture);
    };
} // namespace ce
