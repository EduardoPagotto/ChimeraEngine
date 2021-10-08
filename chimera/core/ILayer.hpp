#ifndef __CHIMERA_ILAYER2__HPP
#define __CHIMERA_ILAYER2__HPP

#include <SDL2/SDL.h>
#include <string>
// https://github.com/Ershany/Arcane-Engine/tree/master/Arcane
namespace Chimera {

class ILayer {
  public:
    virtual void onAttach() = 0;
    virtual void onDeatach() = 0;
    virtual void onUpdate() = 0;
    virtual bool onEvent(const SDL_Event& event) = 0;
    virtual std::string getName() const = 0;
};

} // namespace Chimera
#endif