#include "chimera/render/vbs/RenderableStatic.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableStatic::RenderableStatic(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize) {

    vao = new Core::VertexArray();
    vao->bind();

    Core::VertexBuffer* vbo = new Core::VertexBuffer(Core::BufferType::STATIC);
    vbo->bind();
    vbo->setLayout(vertexDataComponentes());
    vbo->setData(vertexData, vertexSize);
    vbo->unbind();
    vao->push(vbo);

    vao->unbind();

    ibo = new Core::IndexBuffer(indexData, indexSize);

    // init AABB
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(vertexData, vertexSize, indexData, indexSize, min, max, size);
    aabb.setBoundary(min, max);
}

RenderableStatic::~RenderableStatic() {
    delete vao;
    delete ibo;
}

void RenderableStatic::submit(IRenderer3d* renderer) { renderer->submit(this); }

void RenderableStatic::debugDados() {
    glm::vec3 size = this->aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "P[ %.2f, %.2f, %.2f]", size.x, size.y, size.z);
    aabb.render();
}
} // namespace Chimera