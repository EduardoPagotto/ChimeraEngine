#include "chimera_base/CanvasFB.hpp"
#include <cstddef>
#include <cstring>
#include <format>
#include <memory>
#include <stdexcept>

namespace ce {

    CanvaFB::CanvaFB(const std::string& title, int width, int height, bool fullScreen)
        : fullScreen(fullScreen), pixelFormat(SDL_PIXELFORMAT_RGBA8888), title(title) {

        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland")) {
            throw std::runtime_error(std::format("SDL wayland Failed driver: {}", SDL_GetError()));
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Falha SDL_Init:" + std::string(SDL_GetError()));
        }

        this->window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_VULKAN);
        if (this->window == nullptr) {
            throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
        }

        this->renderer = SDL_CreateRenderer(window, NULL);
        if (renderer == nullptr) {
            throw std::runtime_error(std::format("Couldn't create renderer: {})", SDL_GetError()));
        }

        // Necessario pelo wayland
        if (!SDL_RenderPresent(renderer)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render present: %s", SDL_GetError());
        }

        this->texture = SDL_CreateTexture(renderer, this->pixelFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (this->texture == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "create texture: %s", SDL_GetError());
        }

        this->pixelCanvas = std::make_shared<PixelCanvas>(width, height, 0, this->pixelFormat);
    }

    void CanvaFB::before() { this->pixelCanvas->clear(0); }

    void CanvaFB::after() {

        //  Limpa a tela atual
        if (!SDL_RenderClear(renderer)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render clear: %s", SDL_GetError());
        }

        // TODO: Colocar no CMAKE este def
#define FRAMEBUFFER_GPU_SET 1

#ifdef FRAMEBUFFER_GPU_SET
        // Atualiza a textura pela GPU
        if (!SDL_UpdateTexture(texture, nullptr, this->pixelCanvas->getPixelsView().data(),
                               static_cast<int>(this->pixelCanvas->getWithSize()))) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "update texture: %s", SDL_GetError());
        }
#else
        // Copia pela CPU
        char* pix{nullptr};
        int pitch{0};
        SDL_LockTexture(texture, NULL, (void**)&pix, &pitch);

        for (std::size_t i = 0, sp = 0, dp = 0; i < this->pixelCanvas->getHeight();
             i++, dp += this->pixelCanvas->getWidth(), sp += pitch) {

            std::memcpy(pix + sp, this->pixelCanvas->getPixelsView().data() + dp, this->pixelCanvas->getWithSize());
        }

        SDL_UnlockTexture(texture);
        // fim copia
#endif
        // //  Limpa a tela atual
        // if (!SDL_RenderClear(renderer)) {
        //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render clear: %s", SDL_GetError());
        // }

        // Copia a textura
        if (!SDL_RenderTexture(renderer, texture, NULL, NULL)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render texture: %s", SDL_GetError());
        }

        // exibe
        if (!SDL_RenderPresent(renderer)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render present: %s", SDL_GetError());
        }
    }

    void CanvaFB::toggleFullScreen() {
        if (!fullScreen) {

            SDL_GetWindowPosition(window, &posX, &posY);

            SDL_SetWindowPosition(window, 0, 0);
            SDL_SetWindowFullscreen(window, true);

        } else {

            SDL_SetWindowFullscreen(window, false);
            SDL_SetWindowPosition(window, posX, posY);
        }

        fullScreen = !fullScreen;
    }

    CanvaFB::~CanvaFB() {
        SDL_DestroyTexture(this->texture);
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);

        this->pixelCanvas.reset();
    }

    void CanvaFB::reshape(int width, int height) {
        this->pixelCanvas.reset();
        this->pixelCanvas = std::make_shared<PixelCanvas>(width, height, 0, this->pixelFormat);
    }

} // namespace ce
