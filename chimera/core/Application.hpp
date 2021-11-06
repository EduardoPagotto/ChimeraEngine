#pragma once
#include "Canvas.hpp"
#include "Timer.hpp"

namespace Chimera {

class Application {
  public:
    Application(Canvas* canvas);
    virtual ~Application();
    void run();
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;

  protected:
    bool changeStatusFlow(SDL_Event* pEventSDL);
    int eyeIndice;
    uint32_t fps;
    bool pause;
    Canvas* canvas;
    Timer timerFPS;
};
} // namespace Chimera
