#include "TileLayer.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/render/CameraOrthographic.hpp"
#include <glm/gtx/transform.hpp>

TileLayer::TileLayer(Chimera::Shader shader)
    : Chimera::Layer(new Chimera::BatchRender2D(), shader, new Chimera::CameraOrthographic(16.0, -1.0f, 1.0f)) {

    glUseProgram(shader.getID());
    shader.setUniform2f("light_pos", glm::vec2(4.0f, 1.5f));
    glUseProgram(0);
}

TileLayer::~TileLayer() {}

bool TileLayer::onEvent(const SDL_Event& event) {
    using namespace Chimera;
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
            x = event.motion.x;
            y = event.motion.y;
        } break;
    }
    return true;
}

void TileLayer::onRender() {

    glUseProgram(shader.getID());

    // Formula                                (float)(x * sizeW / witdh - (sizeW/2), (float)((sizeH /2) - y * sizeH / height)
    shader.setUniform2f("light_pos", glm::vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(16.0f - y * 32.0f / 540.0f)));

    Layer::onRender();
    glUseProgram(0);
}