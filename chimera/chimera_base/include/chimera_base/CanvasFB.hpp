#pragma once
#include "ICanva.hpp"
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
            : width_(w), height_(h), buffer_(static_cast<size_t>(w * h), c), pixel_format_(p) {}

        virtual ~PixelCanvas() { buffer_.clear(); }

        [[clang::always_inline]] void clear(uint32_t c) {
            // RGBA8888: 0xRRGGBBAA
            std::fill(this->buffer_.begin(), this->buffer_.end(), c);
        }

        [[clang::always_inline]] void setPixel(uint32_t x, uint32_t y, uint32_t c) {
            if (x < this->width_ && y < this->height_) [[likely]] {
                this->buffer_[(y * this->width_) + x] = c;
            }
        }

        [[clang::always_inline]] uint32_t getWidth() const { return width_; }
        [[clang::always_inline]] uint32_t getHeight() const { return height_; }
        [[clang::always_inline]] uint32_t getWithSize() const { return (this->width_ * sizeof(uint32_t)); }
        [[clang::always_inline]] std::span<const uint32_t> getPixelsView() const { return this->buffer_; }
        [[clang::always_inline]] std::span<uint32_t> getPixels() { return this->buffer_; }
        [[clang::always_inline]] SDL_PixelFormat getPixelFormat() { return pixel_format_; }

      private:
        uint32_t width_{800};
        uint32_t height_{600};
        std::vector<uint32_t> buffer_;

        SDL_PixelFormat pixel_format_;
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
        explicit CanvaFB(const std::string& title, int width, int height, bool full_screen = false);
        virtual ~CanvaFB();

        void before() override;
        void after() override;
        void toggleFullScreen() override;
        void reshape(int width, int height) override;

        uint32_t getWidth() const override { return pixel_canvas_->getWidth(); }
        uint32_t getHeight() const override { return pixel_canvas_->getHeight(); }

        [[clang::always_inline]] std::shared_ptr<PixelCanvas> getPixelsCanvas() { return pixel_canvas_; }
        [[clang::always_inline]] SDL_PixelFormat getPixelFormat() { return pixel_format_; }

      private:
        bool full_screen_{false};
        int pos_x_{0};
        int pos_y_{0};

        std::shared_ptr<PixelCanvas> pixel_canvas_;

        SDL_PixelFormat pixel_format_;
        SDL_Texture* texture_{nullptr};
        SDL_Renderer* renderer_{nullptr};
        SDL_Window* window_{nullptr};

        std::string title_;
    };
} // namespace ce
