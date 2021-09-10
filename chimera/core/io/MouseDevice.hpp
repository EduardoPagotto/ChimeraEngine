#ifndef __CHIMERA_CORE_MOUSE_DEVICE__HPP
#define __CHIMERA_CORE_MOUSE_DEVICE__HPP

#include <SDL2/SDL.h>
#include <map>

namespace Chimera::IO {

class MouseDevice {
  public:
    MouseDevice();
    virtual ~MouseDevice();
    uint8_t getButtonState(const uint8_t& indice);

    void update(SDL_MouseButtonEvent* pMb);

  private:
    std::map<uint8_t, uint8_t> buttonState;
};
} // namespace Chimera::IO

#endif