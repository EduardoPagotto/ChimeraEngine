#pragma once
#include <SDL2/SDL.h>

namespace Chimera {

class IStateMachine {
  public:
    virtual ~IStateMachine() {}
    virtual void onAttach() = 0;
    virtual void onDeatach() = 0;
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;
};
} // namespace Chimera
