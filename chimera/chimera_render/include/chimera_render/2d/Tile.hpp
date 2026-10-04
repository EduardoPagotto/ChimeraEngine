#pragma once
#include "Layer.hpp"

namespace ce {

    class Tile : public Layer {

      public:
        Tile(const std::string& name, IRenderer2D* renderer, std::shared_ptr<Shader> shader,
             std::shared_ptr<Camera> camera)
            : Layer(renderer, shader, camera, name) {}

        virtual ~Tile() {}

        virtual void on_attach() override {}
        virtual void on_deatach() override {}
        virtual void on_update(const double& ts) override {}
        virtual void on_event(const SDL_Event& event) override {}
        virtual void on_render() override { Layer::on_render(); }
    };
} // namespace ce
