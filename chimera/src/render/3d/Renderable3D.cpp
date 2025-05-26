#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include <SDL3/SDL.h>

namespace ce {

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
} // namespace ce
