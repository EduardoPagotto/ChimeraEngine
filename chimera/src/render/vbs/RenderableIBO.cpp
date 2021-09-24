
#include "chimera/render/vbs/RenderableIBO.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableIBO::RenderableIBO() : ibo(nullptr) {}

RenderableIBO::~RenderableIBO() {
    if (ibo != nullptr) {
        delete ibo;
        ibo = nullptr;
    }
}

void RenderableIBO::debugDados() {
    glm::vec3 size = this->aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "IBO: %d Faces: %d AABB[ %.2f, %.2f, %.2f]", this->ibo->getBufferID(), this->getSize(), size.x,
                 size.y, size.z);

    aabb.render();
}

void RenderableIBO::initAABB(VertexData* vertexData, const uint32_t& vertexSize) {
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(vertexData, vertexSize, &indexTris[0], indexTris.size(), min, max, size);
    aabb.setBoundary(min, max);
}

void RenderableIBO::addFace(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc) {
    indexTris.push_back(_pa);
    indexTris.push_back(_pb);
    indexTris.push_back(_pc);
}

void RenderableIBO::initIndexBufferObject() { ibo = new Core::IndexBuffer(&indexTris[0], indexTris.size()); }

void RenderableIBO::submit(IRenderer3d* renderer) { renderer->submit(this); }
} // namespace Chimera