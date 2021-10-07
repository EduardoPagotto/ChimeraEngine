#include "chimera/core/io/MouseDevice.hpp"

namespace Chimera {

std::map<uint8_t, uint8_t> MouseDevice::buttonState;

uint8_t MouseDevice::getButtonState(const uint8_t& indice) {
    auto button_iter = MouseDevice::buttonState.find(indice);
    if (button_iter != buttonState.end())
        return button_iter->second;

    return SDL_RELEASED;
}

void MouseDevice::update(SDL_MouseButtonEvent* pMb) { MouseDevice::buttonState[pMb->button] = pMb->state; }
} // namespace Chimera