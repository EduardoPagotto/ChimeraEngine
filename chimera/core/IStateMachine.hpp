#pragma once
#include "chimera/core/ViewProjection.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace ce {

class IStateMachine {
  public:
    IStateMachine(const std::string& name) : name(name) {}
    virtual ~IStateMachine() = default;
    virtual void onAttach() = 0;
    virtual void onDeatach() = 0;
    virtual void onRender() = 0;
    virtual void onUpdate(IViewProjection& vp, const double& ts) = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;
    inline const std::string getName() const { return name; }

  private:
    std::string name;
};
} // namespace ce
