#pragma once
#include "interfaces.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

namespace ce {

    // refs
    // https://forums.libsdl.org/viewtopic.php?p=51664
    // https://jeux.developpez.com/tutoriels/sdl-2/guide-migration/

    /// @brief CanvasFB create a framebuffer SDL standartvwhoitout OpenGL
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20250401
    class CanvasFB : public ServiceBase<ICanva> {

      private:
        bool fullScreen{false};
        int width, height;
        int posX, posY;
        std::string title;
        uint32_t* frame_buffer{nullptr};
        SDL_PixelFormat pixelFormat;
        SDL_Texture* texture{nullptr};
        SDL_Renderer* renderer{nullptr};
        SDL_Window* window{nullptr};

      public:
        CanvasFB(const std::string& title, int width, int height, bool fullScreen = false)
            : fullScreen(fullScreen), width(width), height(height), title(title) {

            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
                throw std::string("Falha SDL_Init:" + std::string(SDL_GetError()));

            window = SDL_CreateWindow(title.c_str(), width, height, 0); // SDL_WINDOW_VULKAN, SDL_WINDOW_OPENGL
            if (!window) {
                throw std::string("Couldn't create window: %s", SDL_GetError());
            }

            renderer = SDL_CreateRenderer(window, NULL); //"vulkan", "opengl"
            if (!renderer) {
                throw std::string("Couldn't create renderer: %s", SDL_GetError());
            }

            // pixelFormat = SDL_PIXELFORMAT_ABGR8888;
            pixelFormat = SDL_PIXELFORMAT_RGBA8888;
            texture = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
            if (!texture) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "create texture: %s", SDL_GetError());
            }

            // texture = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_TARGET, width, height);
            // if (!texture) {
            //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "create texture: %s", SDL_GetError());
            // }
            // Draw a tile to a texture
            // if (!SDL_SetRenderTarget(renderer, texture)) {
            //     SDL_Log("Couldn't use tile texture: %s", SDL_GetError());
            // }

            frame_buffer = new uint32_t[width * height];
        }

        virtual ~CanvasFB() {
            delete[] frame_buffer;
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }

        void before() override {
            // Limpa
            memset(frame_buffer, 0, width * height * sizeof(uint32_t));
        }

        void after() override {

            // TODO: Colocar no CMAKE este def
#define __FRAMEBUFFER_GPU_SET 1

#ifdef __FRAMEBUFFER_GPU_SET
            // Atualiza a textura pela GPU
            if (!SDL_UpdateTexture(texture, NULL, frame_buffer, width * sizeof(uint32_t))) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "update texture: %s", SDL_GetError());
            }
#else
            // Copia pela CPU
            char* pix;
            int pitch;
            SDL_LockTexture(texture, NULL, (void**)&pix, &pitch);
            for (int i = 0, sp = 0, dp = 0; i < height; i++, dp += width, sp += pitch)
                memcpy(pix + sp, frame_buffer + dp, width * 4);
            SDL_UnlockTexture(texture);
            // fim copia
#endif

            //  Limpa a tela atual
            if (!SDL_RenderClear(renderer)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render clear: %s", SDL_GetError());
            }

            // Copia a textura
            if (!SDL_RenderTexture(renderer, texture, NULL, NULL)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render texture: %s", SDL_GetError());
            }

            // exibe
            if (!SDL_RenderPresent(renderer)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "render present: %s", SDL_GetError());
            }
        }

        void toggleFullScreen() override {
            if (fullScreen == false) {

                SDL_GetWindowPosition(window, &posX, &posY);

                SDL_SetWindowPosition(window, 0, 0);
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

            } else {

                SDL_SetWindowFullscreen(window, 0);
                SDL_SetWindowPosition(window, posX, posY);
            }

            fullScreen = !fullScreen;
        }

        void reshape(int width, int height) override {
            this->width = width;
            this->height = height;
        }

        const int getWidth() const override { return width; }

        const int getHeight() const override { return height; }

        uint32_t* getPixels() override { return frame_buffer; }

        SDL_PixelFormat getPixelFormat() override { return pixelFormat; }
    };
} // namespace ce
