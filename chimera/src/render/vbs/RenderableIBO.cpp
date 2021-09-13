
#include "chimera/OpenGLDefs.hpp"
#include "chimera/render/vbs/RenderableIBO.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

RenderableIBO::RenderableIBO() : ibo(nullptr) {}

RenderableIBO::~RenderableIBO() {
    if (ibo != nullptr) {
        delete ibo;
        ibo = nullptr;
    }
}

void RenderableIBO::debugDados() {
    // glm::vec3 pos = this->aabb.getPosition();
    glm::vec3 size = this->aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "P[ %.2f, %.2f, %.2f]", size.x, size.y, size.z);
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

void RenderableIBO::render() {
    // ibo->render();
    ibo->bind();
    glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    ibo->unbind();
}

void RenderableIBO::inject(glm::vec3* eye, Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue) {
    if (aabb.visible(*frustum) == true) {
        renderQueue->push_back(this);

        if (logData == true) {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "IBO: %d Faces: %d", this->ibo->getBufferID(), this->getSize());
            aabb.render();
        }
    }
}
} // namespace Chimera