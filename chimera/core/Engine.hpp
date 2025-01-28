#pragma once
#include "StateStack.hpp"
#include "Timer.hpp"
#include "interfaces.hpp"

namespace ce {
class Engine {
    uint32_t fps = 140;
    Timer timerFPS;
    StateStack stack;
    std::shared_ptr<ICanva> canvas;

  public:
    Engine();
    ~Engine();
    void run();
    StateStack& getStack() { return stack; }
};
} // namespace ce
