#pragma once
#include "Timer.hpp"
#include "chimera/base/StateStack.hpp"
#include "chimera/base/interfaces.hpp"

namespace ce {
    class Engine {
        std::shared_ptr<ICanva> canvas;
        uint32_t fps = 140;
        Timer timerFPS;
        StateStack stack;

      public:
        Engine();
        virtual ~Engine();
        void run();
        StateStack& getStack() { return stack; }
    };
} // namespace ce
