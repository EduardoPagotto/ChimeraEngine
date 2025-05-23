#include "chimera/render/2d/Layer.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace ce {

Layer::Layer(IRenderer2D* renderer, std::shared_ptr<Shader> shader, Camera* camera, const std::string& name)
    : IStateMachine(name), shader(shader), camera(camera), renderer(renderer) {
    GLint texIDs[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                      16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    glUseProgram(shader->getID());
    shader->setUniformU("textures", UValue(std::vector<int>(std::begin(texIDs), std::end(texIDs))));
    glUseProgram(0);
}

Layer::~Layer() {
    for (int i = 0; i < renderables.size(); i++)
        delete renderables[i];
}

void Layer::onRender() {

    renderer->begin(camera);

    RenderCommand rc;
    rc.shader = shader;
    rc.uniforms["pr_matrix"] = UValue(camera->getProjection());
    //  rc.uniforms["textures"] = UValue(32, texIDs);
    renderer->setCommandRender(&rc);

    for (auto renderable : renderables)
        renderable->submit(*renderer);

    renderer->end();
    renderer->flush();
}
} // namespace ce
