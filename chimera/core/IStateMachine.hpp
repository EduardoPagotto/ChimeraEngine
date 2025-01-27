#pragma once
#include <SDL2/SDL.h>
#include <string>

namespace me {

class IStateMachine {
  public:
    virtual ~IStateMachine() = default;
    virtual void onAttach() = 0;
    virtual void onDeatach() = 0;
    virtual void onRender() = 0;
    virtual void onUpdate(const double& ts) = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;
    virtual const std::string getName() = 0;
};
} // namespace me
