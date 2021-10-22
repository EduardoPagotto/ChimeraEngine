
#include "chimera/render/3d/RenderableFace.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableFace::RenderableFace() : ibo(nullptr) {}

RenderableFace::~RenderableFace() {
    if (ibo != nullptr) {
        delete ibo;
        ibo = nullptr;
    }
}

void RenderableFace::debugDados() {
    glm::vec3 size = this->aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "IBO: %d Faces: %d AABB[ %.2f, %.2f, %.2f]", this->ibo->getBufferID(), this->getSize(), size.x,
                 size.y, size.z);

    aabb.render();
}

void RenderableFace::initAABB(VertexData* vertexData, const uint32_t& vertexSize) {
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(vertexData, vertexSize, &indexTris[0], indexTris.size(), min, max, size);
    aabb.setBoundary(min, max);
}

void RenderableFace::addFace(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc) {
    indexTris.push_back(_pa);
    indexTris.push_back(_pb);
    indexTris.push_back(_pc);
}

void RenderableFace::initIndexBufferObject() { ibo = new IndexBuffer(&indexTris[0], indexTris.size()); }

void RenderableFace::submit(Camera* camera, RenderCommand& command, IRenderer3d* renderer) {

    command.renderable = this;
    command.shader = Shader();

    renderer->submit(command);
}
} // namespace Chimera