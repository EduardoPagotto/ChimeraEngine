#pragma once

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
    inline static std::map<uint8_t, uint8_t> buttonState;
    inline static glm::ivec2 pos, rel, wheel;
    inline static uint32_t flag1, flag2;
};
} // namespace Chimera
