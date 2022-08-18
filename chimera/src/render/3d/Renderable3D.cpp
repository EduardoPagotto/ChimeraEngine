#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Renderable3D::~Renderable3D() {
    if (vao) {
        delete vao;
        vao = nullptr;
    }
}

void Renderable3D::setAux(const uint32_t& command, const uint32_t& draw) {
    this->indexAuxCommand = command;
    this->indexAuxDraw = draw;
}

void Renderable3D::draw(const bool& logData) {
    if (logData)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderable3D draw");
}
} // namespace Chimera