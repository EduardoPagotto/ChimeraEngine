#pragma once
#include "StateStack.hpp"
#include "Timer.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/ViewProjection.hpp"
#include "chimera/core/device/ICanva.hpp"

namespace Chimera {
class Engine {
  public:
    Engine(std::shared_ptr<ServiceLocator> serviceLocator);
    virtual ~Engine();
    void run();
    ICanva* getCanvas() const { return canvas.get(); }
    StateStack& getStack() { return stack; }

  private:
    std::shared_ptr<ServiceLocator> serviceLocator;
    std::shared_ptr<ICanva> canvas;
    std::shared_ptr<IViewProjection> vp;

    Registry* registry;

    uint32_t fps = 140;
    Timer timerFPS;
    StateStack stack;
};
} // namespace Chimera
