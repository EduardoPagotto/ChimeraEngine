#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"

namespace Chimera {

Renderable3D::~Renderable3D() {
    if (vao) {
        delete vao;
        vao = nullptr;
    }

    if (ibo) {
        delete ibo;
        ibo = nullptr;
    }
}

void Renderable3D::submit(RenderCommand& command, IRenderer3d& renderer) { renderer.submit(command); }

void Renderable3D::draw(const bool& logData) {
    if (ibo != nullptr) { // Desenhar o IBO
        ibo->bind();
        glDrawElements(GL_TRIANGLES, ibo->getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        if (logData == true) {
            glm::vec3 size = aabb.getSize();
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "IBO ID: %d Faces: %d AABB[%.2f, %.2f, %.2f]", ibo->getBufferID(), ibo->getSize() / 3,
                         size.x, size.y, size.z);

            aabb.debug_render();
        }

        ibo->unbind();
    }
}

} // namespace Chimera