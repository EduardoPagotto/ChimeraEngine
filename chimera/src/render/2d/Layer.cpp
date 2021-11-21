#include "chimera/render/2d/Layer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Layer::Layer(IRenderer2D* renderer, Shader shader, ICamera* camera) : renderer(renderer), shader(shader), camera(camera) {
    GLint texIDs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    shader.enable();
    shader.setUniformArray("textures", 32, texIDs);
    shader.disable();
}

Layer::~Layer() {
    for (int i = 0; i < renderables.size(); i++)
        delete renderables[i];
}

void Layer::onRender() {
    shader.enable();
    shader.setUniform("pr_matrix", camera->getProjectionMatrix()); // passar para o renderer o shade
    renderer->begin(camera);

    for (auto renderable : renderables)
        renderable->submit(renderer);

    renderer->end();
    renderer->flush();

    shader.disable();
}
} // namespace Chimera