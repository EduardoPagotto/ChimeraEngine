#pragma once
#include "Canvas.hpp"
#include "StateStack.hpp"
#include "Timer.hpp"

namespace Chimera {

class Engine {
  public:
    Engine(Canvas* canvas);
    virtual ~Engine();
    void run();
    void pushLayer(IStateMachine* state);
    void pushOverlay(IStateMachine* state);

    Canvas* getCanvas() const { return canvas; }
    const uint32_t getFPS() const { return fps; }
    const int getEye() const { return eyeIndice; }

  protected:
    bool changeStatusFlow(SDL_Event* pEventSDL);
    int eyeIndice;
    uint32_t fps;
    bool pause;
    Canvas* canvas;
    Timer timerFPS;
    StateStack stack;
};
} // namespace Chimera
