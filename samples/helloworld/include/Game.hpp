#pragma once
#include "TileLayer.hpp"
#include "chimera_base/Engine.hpp"
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_core/gl/CanvasGL.hpp"
#include "chimera_render/2d/Label.hpp"
#include <entt/entt.hpp>

class Game : public ce::IStateMachine {
  public:
    Game(std::shared_ptr<entt::registry> registry, ce::Engine* engine);
    virtual ~Game();
    virtual void on_attach() override;
    virtual void on_deatach() override;
    virtual void on_render() override;
    virtual void on_update(const double& ts) override;
    virtual void on_event(const SDL_Event& event) override;
    std::string get_name() const override { return "GAME"; }

  private:
    std::shared_ptr<entt::registry> registry_;
    std::shared_ptr<ce::CanvasGL> canvas_;
    std::shared_ptr<ce::InputManager> input_manager_;
    std::shared_ptr<ce::Shader> shader_;
    std::shared_ptr<TileLayer> layer_;

    ce::Engine* engine_;
    ce::Label* l_fps_;

    int fps_;
};
