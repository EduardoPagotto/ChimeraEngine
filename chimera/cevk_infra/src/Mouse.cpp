#include "cevk_infra/Mouse.hpp"

namespace ce {

    bool Mouse::getButtonState(const uint8_t& indice) noexcept {

        if (this->buttonState.contains(indice)) {
            return buttonState[indice];
        }

        return false;
    }

    glm::ivec2 Mouse::getMoveRel() noexcept {
        if (flag1 != flag2) {
            flag1 = flag2;
            return rel;
        }
        return glm::ivec2(0);
    }

    bool Mouse::getEvent(const SDL_Event& event) noexcept {
        switch (event.type) {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
                this->updateBt(event.button);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                this->updateMv(event.motion);
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                this->updateWl(event.wheel);
                break;
            default:
                break;
        }
        return false;
    }

    void Mouse::updateMv(const SDL_MouseMotionEvent& mv) noexcept {
        this->pos = glm::ivec2(mv.x, mv.y);
        this->rel = glm::ivec2(mv.xrel, mv.yrel);
        flag1++;
    }

} // namespace ce
