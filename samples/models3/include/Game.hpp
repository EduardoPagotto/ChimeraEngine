#pragma once
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_core/bullet/Solid.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include "chimera_render/2d/Label.hpp"
#include "chimera_render/scene/Scene.hpp"

class Game : public ce::IStateMachine {
  public:
    Game(std::shared_ptr<entt::registry> registry, std::shared_ptr<ce::Scene> scene);
    virtual ~Game() = default;

    virtual void on_attach() override;
    virtual void on_deatach() override;
    virtual void on_render() override;
    virtual void on_update(const double& ts) override;
    virtual void on_event(const SDL_Event& event) override;
    std::string get_name() const override { return "GAME"; }

  private:
    std::shared_ptr<entt::registry> registry_;
    std::shared_ptr<ce::Scene> scene_;
    std::shared_ptr<ce::InputManager> input_manager_;
    std::shared_ptr<ce::AssetManager> assets_;

    ce::Solid* p_corpo_rigido_;
    ce::Label* l_fps_;
    int fps_;
};
