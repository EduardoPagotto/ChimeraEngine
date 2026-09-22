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
        virtual void onRender() override;
        virtual std::string getName() const override { return this->name; }

        void add(IRenderable2D* renderable) { renderables.push_back(renderable); }
        std::shared_ptr<Camera> getCamera() const { return camera; };

      protected:
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Camera> camera;

      private:
        IRenderer2D* renderer;
        std::vector<IRenderable2D*> renderables;
        std::string name;
    };
} // namespace ce
