#include "chimera/render/3d/RenderableIBO.hpp"
#include "chimera/core/gl/OpenGLDefs.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"

namespace ce {

RenderableIBO::RenderableIBO(VertexArray* vao, IndexBuffer* ibo, const AABB& aabb)
    : Renderable3D(), ibo(ibo), aabb(aabb) {
    this->vao = vao; // do not delete!! comas from other side
}

RenderableIBO::~RenderableIBO() {
    delete ibo;
    ibo = nullptr;
}

void RenderableIBO::submit(RenderCommand& command, IRenderer3d& renderer) { renderer.submit(command, this, 0); }

void RenderableIBO::draw(const bool& logData) {

    ibo->bind();
    glDrawElements(GL_TRIANGLES, ibo->getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // if (logData == true) {
    //     glm::vec3 size = aabb.getSize();
    //     SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "RenderableIBO ID: %d Faces: %d AABB[%.2f, %.2f, %.2f]",
    //     ibo->getBufferID(),
    //                  ibo->getSize() / 3, size.x, size.y, size.z);
    // }

    ibo->unbind();
}

} // namespace ce
