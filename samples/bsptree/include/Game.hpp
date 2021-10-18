#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "chimera/core/Application.hpp"
#include "chimera/core/Camera.hpp"
#include "chimera/core/Texture.hpp"
#include "chimera/core/space/BSPTree.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/scene/Scene.hpp"

class Game : public Chimera::Application {
  public:
    Game(Chimera::Canvas* canvas);
    virtual ~Game();
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::BspTree bspTree;
    Chimera::Camera* camera;
    Chimera::Shader* pShader;

    Chimera::Renderer3d render3d;
    Chimera::RenderableBsp* renderz1;
    Chimera::Scene activeScene;
};

#endif