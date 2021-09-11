#include "chimera/render/vbs/RenderableStatic3d.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableStatic3d::RenderableStatic3d(VertexData* pVertexList, const unsigned int& vertSize, uint32_t* pIndexList,
                                       const unsigned int& indexSize, Shader* pShader)
    : size(indexSize), pShader(pShader) {

    // create buffers
    this->vao = new Core::VertexArray();
    this->ibo = new Core::IndexBuffer(pIndexList, indexSize);

    // init AABB
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(pVertexList, vertSize, pIndexList, indexSize, min, max, size);
    aabb.setBoundary(min, max);
}

RenderableStatic3d::~RenderableStatic3d() {
    delete ibo;
    ibo = nullptr;
}

void RenderableStatic3d::debugDados() {
    // glm::vec3 pos = this->aabb.getPosition();
    glm::vec3 size = this->aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "P[ %.2f, %.2f, %.2f]", size.x, size.y, size.z);
}

void SimpleRender3d::submit(RenderableStatic3d* renderable) { this->renderQueue.push_back(renderable); }

void SimpleRender3d::flush() {

    while (!renderQueue.empty()) {

        RenderableStatic3d* r = renderQueue.front();
        r->getVao()->bind();
        r->getIBO()->bind();

        // r->getShader()->setUniform1i("teste", 1);
        glDrawElements(GL_TRIANGLES, r->getIBO()->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        r->getIBO()->unbind();
        r->getVao()->unbind();

        renderQueue.pop_front();
    }
}

} // namespace Chimera