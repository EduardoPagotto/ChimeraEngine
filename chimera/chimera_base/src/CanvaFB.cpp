#include "chimera_base/CanvasFB.hpp"
#include <cstddef>
#include <cstring>
#include <format>
#include <stdexcept>

namespace ce {

    CanvaFB::CanvaFB(const std::string& title, int width, int height, bool fullScreen)
        : fullScreen(fullScreen), width(width), height(height), title(title) {

        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland")) {
            throw std::runtime_error(std::format("SDL wayland Failed driver: {}", SDL_GetError()));
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Falha SDL_Init:" + std::string(SDL_GetError()));
        }

        this->window = SDL_CreateWindow(title.c_str(), width, height, 0); // SDL_WINDOW_VULKAN, SDL_WINDOW_OPENGL
        if (this->window == nullptr) {
            throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
        }

        this->renderer = SDL_CreateRenderer(window, NULL); //"vulkan", "opengl"
        if (renderer == nullptr) {
            throw std::runtime_error(std::format("Couldn't create renderer: {})", SDL_GetError()));
        }

        // Necessario pelo wayland
        if (!SDL_RenderPresent(renderer)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render present: %s", SDL_GetError());
        }

        // pixelFormat = SDL_PIXELFORMAT_ARGB8888;
        this->pixelFormat = SDL_PIXELFORMAT_RGBA8888;
        this->texture = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (this->texture == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "create texture: %s", SDL_GetError());
        }

        // frame_buffer = new uint32_t[tot];
        auto tot = static_cast<size_t>(width) * height;
        frame_buffer.resize(tot);
    }

    void CanvaFB::before() { std::memset(frame_buffer.data(), 0, frame_buffer.size()); }

    void CanvaFB::after() {

        //  Limpa a tela atual
        if (!SDL_RenderClear(renderer)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render clear: %s", SDL_GetError());
        }

// TODO: Colocar no CMAKE este def
#define FRAMEBUFFER_GPU_SET 1

#ifdef FRAMEBUFFER_GPU_SET
        // Atualiza a textura pela GPU
        if (!SDL_UpdateTexture(texture, NULL, frame_buffer.data(), static_cast<int>(width * sizeof(uint32_t)))) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "update texture: %s", SDL_GetError());
        }
#else
        // Copia pela CPU
        char* pix;
        int pitch;
        SDL_LockTexture(texture, NULL, (void**)&pix, &pitch);

        for (std::size_t i = 0, sp = 0, dp = 0; i < height; i++, dp += width, sp += pitch) {
            memcpy(pix + sp, frame_buffer.data() + dp, width * 4);
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
        // this->frame_buffer.clear();
    }

    void CanvaFB::reshape(int width, int height) {
        // TODO: implementar
        this->width = width;
        this->height = height;

        frame_buffer.clear();

        auto tot = static_cast<size_t>(width) * height;
        frame_buffer.resize(tot, 0);
    }

} // namespace ce
