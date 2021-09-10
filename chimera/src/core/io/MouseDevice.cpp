#include "chimera/core/io/MouseDevice.hpp"

namespace Chimera::IO {

MouseDevice::MouseDevice() {}
MouseDevice::~MouseDevice() {}
uint8_t MouseDevice::getButtonState(const uint8_t& indice) {
    auto button_iter = buttonState.find(indice);
    if (button_iter != buttonState.end())
        return button_iter->second;

    return SDL_RELEASED;
}

void MouseDevice::update(SDL_MouseButtonEvent* pMb) { this->buttonState[pMb->button] = pMb->state; }
} // namespace Chimera::IO