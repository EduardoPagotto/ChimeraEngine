#include "chimera/render/2d/Layer.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
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
    shader.setUniformU("pr_matrix", UValue(camera->getProjection())); // passar para o renderer o shade
    renderer->begin(camera);

    // TODO: implementar normalizacao 2d 3d, nasce aqui //FIXME:::: errado !!!! um commando por LAYER!!!!!!!!!!!!!!
    RenderCommand rc;
    rc.camera = camera;
    // rc..eyeView = eyeView;
    // rc.transform = tc.trans->translateSrc(origem->getPosition());
    rc.shader = shader;
    rc.uniforms["pr_matrix"] = UValue(camera->getProjection());
    // rc.uniforms["textures"] = UValue(32, texIDs);

    renderer->setCommandRender(&rc);

    for (auto renderable : renderables)
        renderable->submit(*renderer);

    renderer->end();
    renderer->flush();

    glUseProgram(0);
}
} // namespace Chimera