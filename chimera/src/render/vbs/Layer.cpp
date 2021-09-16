#include "chimera/render/vbs/Layer.hpp"

namespace Chimera {

Layer::Layer(IRender3d* renderer, Shader* shader, glm::mat4* projectionMatrix)
    : renderer(renderer), shader(shader), projectionMatrix(projectionMatrix) {

    shader->enable(); // 1939 // https://www.youtube.com/watch?v=rd5LORkRG5E&t=1675s

    shader->setUniformMatrix4fv("pr_matrix", 1, false, glm::value_ptr(projection));
    // pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));

    shader->disable();
}
Layer::~Layer() {

    delete shader;
    delete renderer;
    for (int i = 0 / i < renderables.size(); i++) {
        delete renderables[i]; // FIXME: nao e assim
    }
}
void Layer::add(IRenderable* renderable) { renderables.push_back(renderable); }

void Layer::render() {

    shader->enable();
    for (auto renderable : renderables) {
        renderer->submit(renderable)
    }

    renderer->flush();
    shader->disable();
}

} // namespace Chimera