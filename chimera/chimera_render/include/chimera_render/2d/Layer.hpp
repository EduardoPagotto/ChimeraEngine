#pragma once
#include "Renderable2D.hpp"
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/aux/ICamera.hpp"
#include "chimera_core/gl/Shader.hpp"

namespace ce {

    class Layer : public IStateMachine {
      public:
        Layer(IRenderer2D* renderer, std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera,
              const std::string& name);

        virtual ~Layer();
        virtual void on_render() override;
        virtual std::string get_name() const override { return this->name_; }

        void add(IRenderable2D* renderable) { renderables_.push_back(renderable); }
        std::shared_ptr<Camera> get_camera() const { return camera; };

      protected:
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Camera> camera;

      private:
        IRenderer2D* renderer_;
        std::vector<IRenderable2D*> renderables_;
        std::string name_;
    };
} // namespace ce
