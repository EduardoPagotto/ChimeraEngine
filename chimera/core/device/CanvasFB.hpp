#pragma once
#include "chimera/base/interfaces.hpp"
#include <SDL2/SDL.h>
#include <exception>

namespace ce {

    // refs
    // https://forums.libsdl.org/viewtopic.php?p=51664
    // https://jeux.developpez.com/tutoriels/sdl-2/guide-migration/

    class CanvasFB : public ServiceBase<ICanva> {
      private:
        bool fullScreen{false};
        int width, height;
        int posX, posY;
        std::string title;
        uint32_t* pixels{nullptr};
        uint32_t pixelFormat;
        SDL_Texture* texture{nullptr};
        SDL_Renderer* renderer{nullptr};
        SDL_Window* window{nullptr};

      public:
        CanvasFB(const std::string& title, int width, int height, bool fullScreen = false)
            : fullScreen(fullScreen), width(width), height(height), title(title) {

            if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
                throw std::string("Falha SDL_Init:" + std::string(SDL_GetError()));

            if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) != 0)
                throw std::string("Falha Criar Janela SDL:" + std::string(SDL_GetError()));

            pixelFormat = SDL_PIXELFORMAT_RGBA8888;
            texture = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, width, height);

            pixels = new uint32_t[width * height];
        }

        virtual ~CanvasFB() {
            delete[] pixels;
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }

        virtual void before() override {
            // Limpa
            memset(pixels, 0, width * height * sizeof(uint32_t));
        }

        virtual void after() override {
            // Atualiza a textura
            SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));

            // Limpa a tela atual
            SDL_RenderClear(renderer);

            // Copia a textura
            SDL_RenderCopy(renderer, texture, NULL, NULL);

            // exibe
            SDL_RenderPresent(renderer);
        }

        virtual void toggleFullScreen() override {
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

        virtual void reshape(int width, int height) override {
            this->width = width;
            this->height = height;
        }

        virtual const int getWidth() const override { return width; }

        virtual const int getHeight() const override { return height; }

        uint32_t* getPixels() override { return pixels; }

        uint32_t getPixelFormat() { return pixelFormat; }
    };
} // namespace ce
