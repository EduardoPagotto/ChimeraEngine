#ifndef __GAME_SIMPLES__HPP
#define __GAME_SIMPLES__HPP

#include "chimera/core/Application.hpp"
#include "chimera/core/ICamera.hpp"
#include "chimera/core/Texture.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/vbs/RenderableStatic.hpp"
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
    Chimera::ICamera* camera;
    Chimera::Shader* pShader;
    Chimera::Material material;
    Chimera::RenderableStatic* rederable;
    Chimera::SimpleRender3d render3D;
    Chimera::Frustum frustum;
};

#endif