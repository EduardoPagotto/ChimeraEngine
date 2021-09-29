#ifndef __CHIMERA_BATCHRENDERABLE_2D__HPP
#define __CHIMERA_BATCHRENDERABLE_2D__HPP

#include "Renderable2D.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"

namespace Chimera {

struct VertexDataSimple {
    glm::vec3 vertex;
    glm::vec2 uv;
    float tid;
    glm::vec4 color;
};

#define RENDERER_MAX_SPRITES 60000
#define RENDERER_VERTEX_SIZE sizeof(VertexDataSimple)
#define RENDERER_SPRITE_SIZE RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE RENDERER_SPRITE_SIZE* RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURE 32

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_TID_INDEX 2
#define SHADER_COLOR_INDEX 3

class BatchRender2D : public IRenderer2D {
  public:
    BatchRender2D();
    virtual ~BatchRender2D();

    void init();
    virtual void begin() override;
    virtual void submit(IRenderable2D* renderable) override;
    virtual void end() override;
    virtual void flush() override;

    virtual void drawString(FontAtlas* font, const std::string& text, const glm::vec3& pos, const glm::vec4& color) override;

    inline virtual TransformationStack& getStack() override { return stack; };

  private:
    TransformationStack stack;
    Core::IndexBuffer* ibo;
    GLuint vao;
    GLuint vbo;
    GLsizei indexCount;
    VertexDataSimple* buffer;
    std::vector<GLuint> textureSlots;
};
} // namespace Chimera
#endif
