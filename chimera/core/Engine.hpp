#pragma once
#include "StateStack.hpp"
#include "Timer.hpp"
#include "chimera/core/device/Canvas.hpp"

namespace Chimera {

class Engine {
  public:
    Engine(Canvas* canvas);
    virtual ~Engine();
    void run();
    void pushState(IStateMachine* state);
    void pushOverlay(IStateMachine* state);
    Canvas* getCanvas() const { return canvas; }
    const int getEye() const { return eyeIndice; }
    IStateMachine* getState(const std::string& name) const { return stack.getState(name); }

  protected:
    bool changeStatusFlow(SDL_Event* pEventSDL);
    int eyeIndice;
    bool pause;
    uint32_t fps, countDelta;
    Canvas* canvas;
    Timer timerFPS;
    StateStack stack;
};
} // namespace Chimera
