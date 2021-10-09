#ifndef __CHIMERA_CORE_MOUSE_DEVICE__HPP
#define __CHIMERA_CORE_MOUSE_DEVICE__HPP

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <map>

namespace Chimera {

class MouseDevice {
  public:
    static uint8_t getButtonState(const uint8_t& indice);
    static const glm::ivec2 getMove();
    static const glm::ivec2 getMoveRel();
    static void updateBt(SDL_MouseButtonEvent* bt);
    static void updateMv(SDL_MouseMotionEvent* mv);
    static void updateWl(SDL_MouseWheelEvent* mwe);

  private:
    static std::map<uint8_t, uint8_t> buttonState;
    static glm::ivec2 pos, rel, wheel;
    static uint32_t flag1, flag2;
};
} // namespace Chimera

#endif