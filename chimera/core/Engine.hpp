#pragma once
#include "StateStack.hpp"
#include "Timer.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/ViewProjection.hpp"
#include "chimera/core/device/ICanva.hpp"

namespace Chimera {
class Engine {
  public:
    Engine(Canvas* canvas, const float& dist);
    virtual ~Engine();
    void run();
    Canvas* getCanvas() const { return canvas; }
    StateStack& getStack() { return stack; }

  private:
    Registry* registry;
    ViewProjection vp;
    uint32_t fps = 140;
    Canvas* canvas;
    Timer timerFPS;
    StateStack stack;
};
} // namespace Chimera
