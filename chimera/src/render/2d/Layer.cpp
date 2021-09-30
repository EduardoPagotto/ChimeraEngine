#include "chimera/render/2d/Layer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Layer::Layer(IRenderer2D* renderer, Shader* shader, glm::mat4 projectionMatrix)
    : renderer(renderer), shader(shader), projectionMatrix(projectionMatrix) {

    shader->enable();
    shader->setUniformMatrix4fv("pr_matrix", 1, false, glm::value_ptr(projectionMatrix)); // TODO: ver se da para usar o ard direto!!!!

    GLint texIDs[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  //
                      10, 11, 12, 13, 14, 15, 16, 17, 18, 19, //
                      20, 21, 22, 23, 24, 25, 26, 27, 28, 29, //
                      30, 31};

    shader->setUniform1iv("textures", 32, texIDs);

    shader->disable();
}
Layer::~Layer() {

    delete shader;
    delete renderer;
    for (int i = 0; i < renderables.size(); i++) {
        delete renderables[i]; // FIXME: nao e assim
    }
}

void Layer::render() {

    shader->enable();
    renderer->begin();

    for (auto renderable : renderables)
        renderable->submit(renderer);

    renderer->end();
    renderer->flush();

    shader->disable();
}
} // namespace Chimera