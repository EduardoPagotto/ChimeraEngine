#pragma once
#include "Renderable2D.hpp"
#include "chimera/core/buffer/IndexBuffer.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/render/VertexData.hpp"
#include <deque>

namespace Chimera {

#define RENDERER_MAX_SPRITES 60000
#define RENDERER_SPRITE_SIZE 4
#define RENDERER_BUFFER_SIZE RENDERER_SPRITE_SIZE* RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURE 32

class BatchRender2D : public IRenderer2D {
  public:
    BatchRender2D();
    virtual ~BatchRender2D();

    void init();
    virtual void begin(Camera* camera) override;
    virtual void submit(RenderCommand& command, IRenderable2D* renderable) override;
    virtual void end() override;
    virtual void flush() override;

    virtual void drawString(RenderCommand& command, FontAtlas* font, const std::string& text, const glm::vec3& pos,
                            const glm::vec4& color) override;

    inline virtual TransformationStack& getStack() override { return stack; };

  private:
    float submitTexture(RenderCommand& command, Texture* texture);

    TransformationStack stack;
    IndexBuffer* ibo;
    VertexArray* pVao;
    VertexBuffer* pVbo;
    GLsizei indexCount;
    VertexDataSimple* buffer;
    std::deque<RenderCommand> commandQueue;
    Camera* camera;
};
} // namespace Chimera
