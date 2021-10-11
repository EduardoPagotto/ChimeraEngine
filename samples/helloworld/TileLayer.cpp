#include "TileLayer.hpp"
#include "chimera/core/CameraOrthographic.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include <glm/gtx/transform.hpp>

TileLayer::TileLayer(Chimera::Shader* shader)
    : Chimera::Layer(new Chimera::BatchRender2D(), shader, new Chimera::CameraOrthographic(-16.0f, 16.0f, -9.0f, 9.0f)) {

    GLint texIDs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

    shader->enable();
    shader->setUniform("light_pos", glm::vec2(4.0f, 1.5f));
    shader->setUniformArray("textures", 32, texIDs);
    shader->disable();
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
    return false;
}

void TileLayer::render() {

    shader->enable();
    shader->setUniform("light_pos", glm::vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f)));

    Layer::render();
    shader->disable();
}