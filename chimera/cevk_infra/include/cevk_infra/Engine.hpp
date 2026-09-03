#pragma once

#include "cevk_infra/IScr.hpp"
#include "cevk_infra/StateStack.hpp"
#include "cevk_infra/Timer.hpp"

namespace ce {

    constexpr uint32_t miniumCountDelta = 1000 / 140;

    /// @brief Engine
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260708
    class Engine {

      public:
        Engine(std::shared_ptr<IScr> screen);
        virtual ~Engine() = default;
        void run();

        StateStack& getStack() { return stack; }

      private:
        std::shared_ptr<IScr> screen;
        uint32_t fps = 140;
        Timer timerFPS;
        StateStack stack;
    };
} // namespace ce
