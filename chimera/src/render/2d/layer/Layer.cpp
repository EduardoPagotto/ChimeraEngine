#include "chimera/render/2d/layer/Layer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Layer::Layer(IRenderer2D* renderer, Shader* shader, CameraOrthographic* camera) : renderer(renderer), shader(shader), camera(camera) {}

Layer::~Layer() {

    delete shader;
    delete renderer;
    for (int i = 0; i < renderables.size(); i++) {
        delete renderables[i]; // FIXME: nao e assim
    }
}

void Layer::onRender() {

    shader->enable();
    shader->setUniform("pr_matrix", camera->getProjectionMatrix()); // passar para o renderer o shade
    renderer->begin(camera);

    for (auto renderable : renderables)
        renderable->submit(renderer);

    renderer->end();
    renderer->flush();

    shader->disable();
}
} // namespace Chimera