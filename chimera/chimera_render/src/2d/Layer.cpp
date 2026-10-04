#include "chimera_render/2d/Layer.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace ce {

    Layer::Layer(IRenderer2D* renderer, std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera,
                 const std::string& name)
        : shader(shader), camera(camera), renderer_(renderer), name_(name) {

        GLint texIDs[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                          16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

        glUseProgram(shader->get_id());
        shader->set_uniform_u("textures", Uniform(std::vector<int>(std::begin(texIDs), std::end(texIDs))));
        glUseProgram(0);
    }

    Layer::~Layer() {

        for (int i = 0; i < renderables_.size(); i++) {
            delete renderables_[i];
        }
    }

    void Layer::on_render() {

        renderer_->begin(camera);

        RenderCommand rc;
        rc.shader = shader;
        rc.uniforms["pr_matrix"] = Uniform(camera->get_projection());
        //  rc.uniforms["textures"] = Uniform(32, texIDs);
        renderer_->setCommandRender(&rc);

        for (auto* renderable : renderables_) {
            renderable->submit(*renderer_);
        }

        renderer_->end();
        renderer_->flush();
    }
} // namespace ce
