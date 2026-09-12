#pragma once
#include "chimera_base/ICanva.hpp"
#include <SDL3/SDL.h>
#include <cstddef>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace ce {

    class PixelCanvas {
      public:
        explicit PixelCanvas(uint32_t w, uint32_t h, uint32_t c, SDL_PixelFormat p)
            : width(w), height(h), buffer(static_cast<size_t>(w * h), c), pixelFormat(p) {}

        virtual ~PixelCanvas() { buffer.clear(); }

        [[clang::always_inline]] void clear(uint32_t c) {
            // RGBA8888: 0xRRGGBBAA
            std::fill(this->buffer.begin(), this->buffer.end(), c);
        }

        [[clang::always_inline]] void setPixel(uint32_t x, uint32_t y, uint32_t c) {
            if (x < this->width && y < this->height) [[likely]] {
                this->buffer[(y * this->width) + x] = c;
            }
        }

        [[clang::always_inline]] uint32_t getWidth() const { return width; }
        [[clang::always_inline]] uint32_t getHeight() const { return height; }
        [[clang::always_inline]] uint32_t getWithSize() const { return (this->width * sizeof(uint32_t)); }
        [[clang::always_inline]] std::span<const uint32_t> getPixelsView() const { return this->buffer; }
        [[clang::always_inline]] std::span<uint32_t> getPixels() { return this->buffer; }
        [[clang::always_inline]] SDL_PixelFormat getPixelFormat() { return pixelFormat; }

      private:
        uint32_t width{800};
        uint32_t height{600};
        std::vector<uint32_t> buffer;

        SDL_PixelFormat pixelFormat;
    };

    // refs
    // https://forums.libsdl.org/viewtopic.php?p=51664
    // https://jeux.developpez.com/tutoriels/sdl-2/guide-migration/

    /// @brief CanvaFB create a framebuffer SDL standartvwhoitout OpenGL
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260911
    class CanvaFB : public ICanva {
      public:
        explicit CanvaFB(const std::string& title, int width, int height, bool fullScreen = false);
        virtual ~CanvaFB();

        void before() override;
        void after() override;
        void toggleFullScreen() override;
        void reshape(int width, int height) override;

        uint32_t getWidth() const override { return pixelCanvas->getWidth(); }
        uint32_t getHeight() const override { return pixelCanvas->getHeight(); }

        [[clang::always_inline]] std::shared_ptr<PixelCanvas> getPixelsCanvas() { return pixelCanvas; }
        [[clang::always_inline]] SDL_PixelFormat getPixelFormat() { return pixelFormat; }

      private:
        bool fullScreen{false};
        int posX{0};
        int posY{0};

        std::shared_ptr<PixelCanvas> pixelCanvas;

        SDL_PixelFormat pixelFormat;
        SDL_Texture* texture{nullptr};
        SDL_Renderer* renderer{nullptr};
        SDL_Window* window{nullptr};

        std::string title;
    };
} // namespace ce
