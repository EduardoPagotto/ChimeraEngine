#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace ce {

    /// @brief Interface State Machine
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20270731
    class IStateMachine {
      public:
        virtual ~IStateMachine() = default;
        virtual void onAttach() = 0;
        virtual void onDeatach() = 0;
        virtual void onRender() = 0;
        virtual void onUpdate(const double& ts) = 0;
        virtual bool onEvent(const SDL_Event& event) = 0;
        virtual std::string getName() const = 0;
    };
} // namespace ce
