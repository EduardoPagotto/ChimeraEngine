#pragma once
#include "StateStack.hpp"
#include "Timer.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/ViewProjection.hpp"
#include "chimera/core/device/ICanva.hpp"

namespace Chimera {
class Engine {
  public:
    Engine(ICanva* canvas, const float& dist);
    virtual ~Engine();
    void run();
    ICanva* getCanvas() const { return canvas; }
    StateStack& getStack() { return stack; }

  private:
    Registry* registry;
    ViewProjection vp;
    uint32_t fps = 140;
    ICanva* canvas;
    Timer timerFPS;
    StateStack stack;
};
} // namespace Chimera
