#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace ce {

    /// @brief Interface State Machine
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20261004
    class IStateMachine {
      public:
        virtual ~IStateMachine() = default;
        virtual void on_attach() = 0;
        virtual void on_deatach() = 0;
        virtual void on_render() = 0;
        virtual void on_update(const double& ts) = 0;
        virtual void on_event(const SDL_Event& event) = 0;
        virtual std::string get_name() const = 0;
    };
} // namespace ce
