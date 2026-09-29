#pragma once
#include "chimera_base/CanvasFB.hpp"
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include "raycasting.hpp"
#include <entt/entt.hpp>

class Game : public ce::IStateMachine {
  public:
    Game(std::shared_ptr<entt::registry> registry);
    virtual ~Game();

    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual void onEvent(const SDL_Event& event) override;
    virtual std::string getName() const override;

  private:
    void testeGamePad();

    State* state_{nullptr};
    World* world_{nullptr};
    float move_speed_{0.0F};
    float rot_speed_{0.0F};
    std::shared_ptr<entt::registry> registry_;
    std::shared_ptr<ce::CanvaFB> canva_;
    std::shared_ptr<ce::InputManager> input_manager_;

    ce::Gamepad::AxixConfig player0_config_{0.18F, 0.18F, 0.18F}; // Deadzones customizadas
};
