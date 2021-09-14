#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/vbs/RenderableIndex.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

class Game : public Chimera::IO::IEvents {
  public:
    Game();
    virtual ~Game();
    // Inherited via IEvents
    virtual void start();
    virtual void update();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::IO::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::IO::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

  private:
    bool debugParser;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    Chimera::TrackBall trackBall;
    Chimera::CanvasGL* pCanvas;
    Chimera::Shader* pShader;
    Chimera::Light* pLight;
    Chimera::Material* pMaterial;
    Chimera::RenderableIndex* pHeightMap;
    Chimera::SimpleRender3d render3d;
    Chimera::Frustum frustum;
};

#endif