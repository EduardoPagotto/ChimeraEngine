#ifndef __HELLOWORD_TEST__HPP
#define __HELLOWORD_TEST__HPP

#include "TileLayer.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/render/2d/Layer.hpp"
#include "chimera/render/FontAtlas.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::Core::CanvasGL* _pVideo);
    virtual ~Game();
    void start();
    virtual void update();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::Core::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::Core::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

  private:
    Chimera::Core::CanvasGL* pVideo;
    Chimera::Shader *shader, *shader2;
    TileLayer *layer, *layer2;
    uint16_t x, y;

    Chimera::FontAtlas* fa;
};

#endif