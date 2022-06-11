#pragma once
#include "StateStack.hpp"
#include "Timer.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/device/Canvas.hpp"

namespace Chimera {

class Engine {
  public:
    Engine(Canvas* canvas);
    virtual ~Engine();
    void init();
    void run();
    Canvas* getCanvas() const { return canvas; }
    StateStack& getStack() { return stack; }
    Registry& getRegistry() { return registry; }

  private:
    Registry registry;
    bool changeStatusFlow(SDL_Event* pEventSDL);
    bool pause;
    uint32_t fps, countDelta;
    Canvas* canvas;
    Timer timerFPS;
    StateStack stack;
};
} // namespace Chimera
