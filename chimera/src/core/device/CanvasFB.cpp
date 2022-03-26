#include "chimera/core/device/CanvasFB.hpp"
#include <exception>

namespace Chimera {

// refs
// https://forums.libsdl.org/viewtopic.php?p=51664
// https://jeux.developpez.com/tutoriels/sdl-2/guide-migration/

CanvasFB::CanvasFB(const std::string& _title, int _width, int _height, bool _fullScreen) : Canvas(_title, _width, _height, _fullScreen) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        throw std::string("Falha SDL_Init:" + std::string(SDL_GetError()));

    if (SDL_CreateWindowAndRenderer(_width, _height, 0, &window, &renderer) != 0)
        throw std::string("Falha Criar Janela SDL:" + std::string(SDL_GetError()));

    pixelFormat = SDL_PIXELFORMAT_RGBA8888;
    texture = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, _width, _height);

    pixels = new uint32_t[_width * _height];
}

CanvasFB::~CanvasFB() {
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void CanvasFB::before(const unsigned short& _indexEye) {
    // Limpa
    memset(pixels, 0, width * height * sizeof(uint32_t));
}

void CanvasFB::after(const unsigned short& _indexEye) {

    // Atualiza a textura
    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));

    // Limpa a tela atual
    SDL_RenderClear(renderer);

    // Copia a textura
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // exibe
    SDL_RenderPresent(renderer);
}

void CanvasFB::toggleFullScreen() {

    if (fullScreen == false) {

        SDL_GetWindowPosition(window, &posX, &posY);

        SDL_SetWindowPosition(window, 0, 0);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    } else {

        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowPosition(window, posX, posY);
    }

    fullScreen = !fullScreen;
}

void CanvasFB::reshape(int _width, int _height) {
    width = _width;
    height = _height;
}
} // namespace Chimera
