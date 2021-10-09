#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "chimera/core/Application.hpp"
#include "chimera/core/ICamera.hpp"
#include "chimera/core/Texture.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/core/space/BSPTree.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

class Game : public Chimera::Application {
  public:
    Game(Chimera::Canvas* canvas);
    virtual ~Game();
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::BspTree bspTree;
    Chimera::ICamera* camera;
    Chimera::Shader* pShader;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    Chimera::Texture* pTex;
    Chimera::Frustum frustum;
    Chimera::SimpleRender3d render3d;
    Chimera::RenderableBsp* renderz1;
};

#endif