#ifndef __GAME_SIMPLES__HPP
#define __GAME_SIMPLES__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/Texture.hpp"
#include "chimera/render/vbs/RenderableStatic.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader);

    virtual ~Game();
    // Inherited via IEvents
    virtual void start();
    virtual void update();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::Core::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::Core::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

  private:
    int debug_init;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    Chimera::TrackBall trackBall;
    Chimera::CanvasGL* pCanvas;
    Chimera::Shader* pShader;
    Chimera::Material material;
    Chimera::RenderableStatic* rederable;
    Chimera::SimpleRender3d render3D;
};

#endif