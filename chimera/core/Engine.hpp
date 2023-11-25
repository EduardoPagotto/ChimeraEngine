#pragma once
#include "StateStack.hpp"
#include "Timer.hpp"
#include "chimera/core/ViewProjection.hpp"
#include "chimera/core/device/Canvas.hpp"

namespace Chimera {

class Engine {
  public:
    Engine(Canvas* canvas, const float& dist);
    virtual ~Engine() = default;
    void init();
    void run();
    Canvas* getCanvas() const { return canvas; }
    StateStack& getStack() { return stack; }

  private:
    ViewProjection vp;
    uint32_t fps = 140;
    Canvas* canvas;
    Timer timerFPS;
    StateStack stack;
};
} // namespace Chimera
