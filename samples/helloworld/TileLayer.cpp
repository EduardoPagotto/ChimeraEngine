#include "TileLayer.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include <glm/gtx/transform.hpp>

TileLayer::TileLayer(Chimera::Shader shader)
    : Chimera::Layer(new Chimera::BatchRender2D(), shader, new Chimera::Camera(16.0, 16.0, -1.0f, 1.0f)) {
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

// FIXME: IMPORTANTE!!! E necessario para movimentacao de matrizes em 2D!!!!!!!!!!!!
// // void CameraOrthographic::updateEye(EyeView& eye) {
// //     glm::mat4 transform =
// //         glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
// //     eye.update(glm::inverse(transform), projectionMatrix);
// // }

void TileLayer::onRender() {
    glUseProgram(shader.getID());

    // Formula                                (float)(x * sizeW / witdh - (sizeW/2), (float)((sizeH /2) - y * sizeH / height)
    shader.setUniform2f("light_pos", glm::vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(16.0f - y * 32.0f / 540.0f)));

    Layer::onRender();
    glUseProgram(0);
}