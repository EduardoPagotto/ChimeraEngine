#include "chimera/render/vbs/RenderableStatic.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableStatic::RenderableStatic(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize) {

    vao = new Core::VertexArray();
    vao->addBuffer(new Core::VertexBuffer(vertexData, vertexSize, 3), 0); // FIXME 0 para compatibilidade

    vao->bind();
    ibo = new Core::IndexBuffer(indexData, indexSize);
    vao->unbind();

    // init AABB
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(vertexData, vertexSize, indexData, indexSize, min, max, size);
    aabb.setBoundary(min, max);
}

RenderableStatic::~RenderableStatic() {
    delete vao;
    delete ibo;
}

void RenderableStatic::inject(glm::vec3* eye, Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue) {
    // TODO: colocar validacao do frustum
    // if (aabb.visible(*frustum) == true) {
    //     renderQueue->push_back(this);
    // }

    renderQueue->push_back(this);
}

void RenderableStatic::debugDados() {
    glm::vec3 size = this->aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "P[ %.2f, %.2f, %.2f]", size.x, size.y, size.z);
}
} // namespace Chimera