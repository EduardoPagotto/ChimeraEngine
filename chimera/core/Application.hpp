#ifndef __CHIMERA_APPLICATION_CONTROLLER__HPP
#define __CHIMERA_APPLICATION_CONTROLLER__HPP

#include "ShaderLibrary.hpp"
#include "io/LayerStack.hpp"
#include "io/Timer.hpp"
#include "windows/Canvas.hpp"

namespace Chimera {

class Application {
  public:
    Application(Canvas* canvas);
    virtual ~Application();
    void run();

  protected:
    Canvas* canvas;
    ShaderLibrary shaderLibrary;

  private:
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;

    bool changeStatusFlow(SDL_Event* pEventSDL);
    LayerStack layerStack;
    Timer timerFPS;
    uint32_t fps;
    bool pause;
};
} // namespace Chimera

#endif