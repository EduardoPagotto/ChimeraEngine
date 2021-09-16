#ifndef __HELLOWORD_TEST__HPP
#define __HELLOWORD_TEST__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/render/vbs/RenderableStatic.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

class Game : public Chimera::IO::IEvents {
  public:
    Game(Chimera::CanvasGL* _pVideo);
    virtual ~Game();
    void start();
    virtual void update();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::IO::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::IO::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

  private:
    Chimera::CanvasGL* pVideo;
    Chimera::Shader* shader;
    Chimera::RenderableStatic* renderable;
    Chimera::SimpleRender3d reder3d;
};

#endif