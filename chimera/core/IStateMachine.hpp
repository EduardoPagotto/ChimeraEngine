#pragma once
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

class IStateMachine {
  public:
    virtual ~IStateMachine() {}
    virtual void onAttach() = 0;
    virtual void onDeatach() = 0;
    virtual void onRender() = 0;
    virtual void onUpdate(const double& ts) = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;
    virtual std::string getName() const = 0;
};
} // namespace Chimera
