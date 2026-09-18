#pragma once
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_core/gl/CanvasGL.hpp"
#include <entt/entt.hpp>
#include <memory>

class Game : public ce::IStateMachine {
  public:
    explicit Game(std::shared_ptr<entt::registry> registry);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual void onEvent(const SDL_Event& event) override;
    std::string getName() const override { return "GAME"; }

  private:
    std::shared_ptr<entt::registry> registry;
    std::shared_ptr<ce::CanvasGL> canva;
    std::shared_ptr<ce::InputManager> inputManager;
};
