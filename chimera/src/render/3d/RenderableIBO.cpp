#include "chimera/render/3d/RenderableIBO.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"

namespace Chimera {

RenderableIBO::~RenderableIBO() {
    delete ibo;
    ibo = nullptr;
}

void RenderableIBO::submit(RenderCommand& command, IRenderer3d& renderer) { renderer.submit(command, this); }

void RenderableIBO::draw(const bool& logData) {

    ibo->bind();
    glDrawElements(GL_TRIANGLES, ibo->getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    if (logData == true) {
        glm::vec3 size = aabb.getSize();
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "RenderableIBO ID: %d Faces: %d AABB[%.2f, %.2f, %.2f]", ibo->getBufferID(),
                     ibo->getSize() / 3, size.x, size.y, size.z);

        aabb.debug_render();
    }

    ibo->unbind();
}

} // namespace Chimera