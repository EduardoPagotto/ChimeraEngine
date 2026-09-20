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
    virtual ~Game();

    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual void onEvent(const SDL_Event& event) override;
    std::string getName() const override { return "GAME"; }

  private:
    std::shared_ptr<entt::registry> registry;
    std::shared_ptr<ce::Scene> scene;
    std::shared_ptr<ce::InputManager> inputManager;
    std::shared_ptr<ce::AssetManager> assets;

    ce::Solid* pCorpoRigido;
    ce::Label* lFPS;
    int fps;
};
