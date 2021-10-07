#ifndef __GAME_TESTE_TERRAIN__HPP
#define __GAME_TESTE_TERRAIN__HPP

#include "chimera/core/Application.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/vbs/RenderableIndex.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

class Game : public Chimera::Application {
  public:
    Game(Chimera::Canvas* canvas);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    Chimera::TrackBall trackBall;
    Chimera::Shader* pShader;
    Chimera::Light* pLight;
    Chimera::Material* pMaterial;
    Chimera::RenderableIndex* pHeightMap;
    Chimera::SimpleRender3d render3d;
    Chimera::Frustum frustum;
};

#endif