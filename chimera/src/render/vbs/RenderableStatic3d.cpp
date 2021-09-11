#include "chimera/render/vbs/RenderableStatic3d.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableStatic3d::RenderableStatic3d(VertexData* pVertexList, const unsigned int& vertSize, uint32_t* pIndexList,
                                       const unsigned int& indexSize) {

    // create buffers
    this->ibo = new Core::IndexBuffer(pIndexList, indexSize);

    // init AABB
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(pVertexList, vertSize, pIndexList, indexSize, min, max, size);
    aabb.setBoundary(min, max);

    this->size = indexSize;
}

RenderableStatic3d::~RenderableStatic3d() {
    delete ibo;
    ibo = nullptr;
}

void RenderableStatic3d::render() { ibo->render(); }

void RenderableStatic3d::debugDados() {
    // glm::vec3 pos = this->aabb.getPosition();
    glm::vec3 size = this->aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "P[ %.2f, %.2f, %.2f]", size.x, size.y, size.z);
}
} // namespace Chimera