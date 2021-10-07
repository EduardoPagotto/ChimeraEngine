#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/vbs/RenderableIndex.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game();
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
    Chimera::Core::CanvasGL* pCanvas;
    Chimera::Shader* pShader;
    Chimera::Light* pLight;
    Chimera::Material* pMaterial;
    Chimera::RenderableIndex* pHeightMap;
    Chimera::SimpleRender3d render3d;
    Chimera::Core::Frustum frustum;
};

#endif