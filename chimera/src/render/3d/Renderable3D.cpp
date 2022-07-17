#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"
#include "chimera/render/VertexData.hpp"

namespace Chimera {

Renderable3D::~Renderable3D() {
    if (vao) {
        delete vao;
        vao = nullptr;
    }
}

void Renderable3D::draw(const bool& logData) {
    if (logData)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderable3D draw");
}

} // namespace Chimera