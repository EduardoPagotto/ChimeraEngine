#include "chimera/render/2d/Layer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Layer::Layer(IRenderer2D* renderer, Shader shader, Camera* camera) : renderer(renderer), shader(shader), camera(camera) {
    GLint texIDs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    glUseProgram(shader.getID());
    shader.setUniformU("textures", UValue(32, texIDs));
    glUseProgram(0);
}

Layer::~Layer() {
    for (int i = 0; i < renderables.size(); i++)
        delete renderables[i];
}

void Layer::onRender() {
    glUseProgram(shader.getID());
    shader.setUniformU("pr_matrix", Chimera::UValue(camera->getProjection())); // passar para o renderer o shade
    renderer->begin(camera);

    for (auto renderable : renderables)
        renderable->submit(*renderer);

    renderer->end();
    renderer->flush();

    glUseProgram(0);
}
} // namespace Chimera