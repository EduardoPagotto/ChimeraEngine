#pragma once
#include "TileLayer.hpp"
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_core/gl/CanvasGL.hpp"
#include "chimera_render/2d/Label.hpp"
#include <entt/entt.hpp>
// #include "chimera_base/Engine.hpp"

class Game : public ce::IStateMachine {
  public:
    Game(std::shared_ptr<entt::registry> registry);
    virtual ~Game();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual void onEvent(const SDL_Event& event) override;
    std::string getName() const override { return "GAME"; }

  private:
    std::shared_ptr<entt::registry> registry;
    std::shared_ptr<ce::CanvasGL> canvas;
    std::shared_ptr<ce::InputManager> inputManager;
    std::shared_ptr<ce::Shader> shader;

    // ce::Engine* engine;
    ce::Label* lFPS;
    TileLayer* layer;
    int fps;
};
