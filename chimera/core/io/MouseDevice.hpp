#ifndef __CHIMERA_CORE_MOUSE_DEVICE__HPP
#define __CHIMERA_CORE_MOUSE_DEVICE__HPP

#include <SDL2/SDL.h>
#include <map>

namespace Chimera::Core {

class MouseDevice {
  public:
    static uint8_t getButtonState(const uint8_t& indice);
    static void update(SDL_MouseButtonEvent* pMb);

  private:
    static std::map<uint8_t, uint8_t> buttonState;
};
} // namespace Chimera::Core

#endif