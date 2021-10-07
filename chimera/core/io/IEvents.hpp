#ifndef __CHIMERA_CORE_IEVENTS__HPP
#define __CHIMERA_CORE_IEVENTS__HPP

#include <SDL2/SDL.h>
// https://github.com/Ershany/Arcane-Engine/tree/master/Arcane
namespace Chimera {

class IEvents {
  public:
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;
};

} // namespace Chimera
#endif
