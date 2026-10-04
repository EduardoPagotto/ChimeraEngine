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
    virtual void on_attach() override;
    virtual void on_deatach() override;
    virtual void on_render() override;
    virtual void on_update(const double& ts) override;
    virtual void on_event(const SDL_Event& event) override;
    virtual std::string get_name() const override;

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
