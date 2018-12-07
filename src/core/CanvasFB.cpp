#include "chimera/core/CanvasFB.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

// refs
// https://forums.libsdl.org/viewtopic.php?p=51664
// https://jeux.developpez.com/tutoriels/sdl-2/guide-migration/

CanvasFB::CanvasFB(const std::string& _title, int _width, int _height, bool _fullScreen)
    : Canvas(_title, _width, _height, _fullScreen) {

    if (SDL_CreateWindowAndRenderer(_width, _height, 0, &window, &renderer) != 0)
        throw Exception(std::string(std::string("Falha Criar Janela SDL:") + SDL_GetError()));

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, _width, _height);

    pixels = new uint32_t[_width * _height];

    log->debug("Instanciado CanvasFrameBuffer");
}

CanvasFB::~CanvasFB() {
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void CanvasFB::before() {
    // Limpa
    memset(pixels, 0, width * height * sizeof(uint32_t));
}

void CanvasFB::after() {

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

        SDL_GetWindowPosition(window, &winPosPrev.x, &winPosPrev.y);

        SDL_SetWindowPosition(window, 0, 0);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    } else {

        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowPosition(window, winPosPrev.x, winPosPrev.y);
    }

    fullScreen = !fullScreen;
}

void CanvasFB::reshape(int _width, int _height) {
    width = _width;
    height = _height;
}
} // namespace Chimera
