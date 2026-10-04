#pragma once

#include "ICanva.hpp"
#include "StateStack.hpp"
#include "Timer.hpp"
#include <entt/entt.hpp>

namespace ce {

    constexpr uint32_t minium_count_delta = 1000 / 140;

    /// @brief Engine
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260917
    class Engine {

      public:
        explicit Engine(std::shared_ptr<entt::registry> registry);
        virtual ~Engine() = default;
        void run();

        StateStack& getStack() { return stack_; }

      private:
        std::shared_ptr<entt::registry> registry_;
        std::shared_ptr<ICanva> canva_;
        uint32_t fps_ = 140;
        Timer timer_fps_;
        StateStack stack_;
    };
} // namespace ce
