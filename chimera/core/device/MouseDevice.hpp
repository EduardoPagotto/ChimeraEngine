#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <map>

namespace Chimera {

class MouseDevice {
  public:
    static uint8_t getButtonState(const uint8_t& indice) {
        auto button_iter = MouseDevice::buttonState.find(indice);
        if (button_iter != buttonState.end())
            return button_iter->second;

        return SDL_RELEASED;
    }

    static const glm::ivec2 getMove() { return pos; }

    static const glm::ivec2 getMoveRel() {
        if (flag1 != flag2) {
            flag1 = flag2;
            return rel;
        }
        return glm::ivec2(0);
    }

    static void updateBt(const SDL_MouseButtonEvent& bt) { MouseDevice::buttonState[bt.button] = bt.state; }

    static void updateMv(const SDL_MouseMotionEvent& mv) {
        MouseDevice::pos = glm::ivec2(mv.x, mv.y);
        MouseDevice::rel = glm::ivec2(mv.xrel, mv.yrel);
        flag1++;
    }

    static void updateWl(const SDL_MouseWheelEvent& mwe) { MouseDevice::wheel = glm::ivec2(mwe.x, mwe.y); }

    static bool getEvent(const SDL_Event& event) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                MouseDevice::updateBt(event.button);
                break;
            case SDL_MOUSEMOTION:
                MouseDevice::updateMv(event.motion);
                break;
            case SDL_MOUSEWHEEL:
                MouseDevice::updateWl(event.wheel);
                break;
        }

        return false;
    }

  private:
    inline static std::map<uint8_t, uint8_t> buttonState;
    inline static glm::ivec2 pos, rel, wheel;
    inline static uint32_t flag1, flag2;
};
} // namespace Chimera
