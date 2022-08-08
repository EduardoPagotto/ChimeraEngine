#pragma once
#include "chimera/core/ViewProjection.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

class IStateMachine {
  public:
    virtual void onAttach() = 0;
    virtual void onDeatach() = 0;
    virtual void onRender() = 0;
    virtual void onUpdate(ViewProjection& vp, const double& ts) = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;
    virtual std::string getName() const = 0;
};
} // namespace Chimera
