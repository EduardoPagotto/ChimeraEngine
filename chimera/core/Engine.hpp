#pragma once
#include "StateStack.hpp"
#include "Timer.hpp"
#include "chimera/core/ViewProjection.hpp"
#include "chimera/core/device/interfaces.hpp"

namespace ce {
class Engine {
  public:
    Engine();
    virtual ~Engine();
    void run();
    // ICanva* getCanvas() const { return canvas.get(); }
    StateStack& getStack() { return stack; }

  private:
    std::shared_ptr<ICanva> canvas;
    std::shared_ptr<IViewProjection> vp;

    uint32_t fps = 140;
    Timer timerFPS;
    StateStack stack;
};
} // namespace ce
