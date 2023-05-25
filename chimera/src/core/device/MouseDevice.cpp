#include "chimera/core/device/MouseDevice.hpp"

namespace Chimera {

uint8_t MouseDevice::getButtonState(const uint8_t& indice) {
    auto button_iter = MouseDevice::buttonState.find(indice);
    if (button_iter != buttonState.end())
        return button_iter->second;

    return SDL_RELEASED;
}

void MouseDevice::updateBt(SDL_MouseButtonEvent* bt) { MouseDevice::buttonState[bt->button] = bt->state; }

void MouseDevice::updateMv(SDL_MouseMotionEvent* mv) {
    MouseDevice::pos = glm::ivec2(mv->x, mv->y);
    MouseDevice::rel = glm::ivec2(mv->xrel, mv->yrel);
    flag1++;
}

void MouseDevice::updateWl(SDL_MouseWheelEvent* mwe) { MouseDevice::wheel = glm::ivec2(mwe->x, mwe->y); }

const glm::ivec2 MouseDevice::getMove() { return pos; }

const glm::ivec2 MouseDevice::getMoveRel() {
    if (flag1 != flag2) {
        flag1 = flag2;
        return rel;
    }
    return glm::ivec2(0);
}
} // namespace Chimera