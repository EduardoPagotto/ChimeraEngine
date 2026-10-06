#pragma once
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_core/gl/CanvasGL.hpp"
#include <entt/entt.hpp>
#include <memory>

class Game : public ce::IStateMachine {
  public:
    explicit Game(std::shared_ptr<entt::registry> registry);
    virtual ~Game() = default;
    virtual void on_attach() override;
    virtual void on_deatach() override;
    virtual void on_render() override;
    virtual void on_update(const double& ts) override;
    virtual void on_event(const SDL_Event& event) override;
    std::string get_name() const override { return "GAME"; }

  private:
    std::shared_ptr<entt::registry> registry_;
    std::shared_ptr<ce::CanvasGL> canva_;
    std::shared_ptr<ce::InputManager> input_manager_;
};
