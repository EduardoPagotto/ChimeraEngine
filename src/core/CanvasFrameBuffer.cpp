//#include "chimera/core/Exception.hpp"
#include "chimera/core/FrameBuffer.hpp"

namespace Chimera {

CanvasFrameBuffer::CanvasFrameBuffer(const std::string& _title, int _width, int _height, bool _fullScreen)
    : Canvas(_title, _width, _height, _fullScreen) {

    log->debug("Instanciado CanvasFrameBuffer");
}

CanvasFrameBuffer::~CanvasFrameBuffer() {}
void CanvasFrameBuffer::before() {}
void CanvasFrameBuffer::after() {}

void CanvasFrameBuffer::toggleFullScreen() {

    if (fullScreen == false) {

        SDL_GetWindowPosition(window, &winPosPrev.x, &winPosPrev.y);

        SDL_SetWindowPosition(window, 0, 0);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    } else {

        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowPosition(window, winPosPrev.x, winPosPrev.y);
    }

    fullScreen = !fullScreen;
}

void CanvasFrameBuffer::reshape(int _width, int _height) {
    width = _width;
    height = _height;
}
} // namespace Chimera
