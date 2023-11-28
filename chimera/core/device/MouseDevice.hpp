#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <map>

namespace Chimera {
class MouseDevice {
  public:
    MouseDevice() = default;
    virtual ~MouseDevice() = default;

    inline uint8_t const getButtonState(const uint8_t& indice) {

        if (this->buttonState.contains(indice))
            return buttonState[indice];

        return SDL_RELEASED;
    }

    inline const glm::ivec2 getMove() const { return pos; }

    inline const glm::ivec2 getMoveRel() {
        if (flag1 != flag2) {
            flag1 = flag2;
            return rel;
        }
        return glm::ivec2(0);
    }

    inline void updateBt(const SDL_MouseButtonEvent& bt) { this->buttonState[bt.button] = bt.state; }

    inline void updateMv(const SDL_MouseMotionEvent& mv) {
        this->pos = glm::ivec2(mv.x, mv.y);
        this->rel = glm::ivec2(mv.xrel, mv.yrel);
        flag1++;
    }

    inline void updateWl(const SDL_MouseWheelEvent& mwe) { this->wheel = glm::ivec2(mwe.x, mwe.y); }

    inline bool getEvent(const SDL_Event& event) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                this->updateBt(event.button);
                break;
            case SDL_MOUSEMOTION:
                this->updateMv(event.motion);
                break;
            case SDL_MOUSEWHEEL:
                this->updateWl(event.wheel);
                break;
        }
        return false;
    }

  private:
    std::map<uint8_t, uint8_t> buttonState;
    glm::ivec2 pos, rel, wheel;
    uint32_t flag1, flag2;
};
} // namespace Chimera
