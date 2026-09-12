#pragma once
#include "chimera_base/ICanva.hpp"
#include <SDL3/SDL.h>
#include <string>
#include <vector>

namespace ce {

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

        uint32_t getWidth() const override { return width; }
        uint32_t getHeight() const override { return height; }

        uint32_t* getPixels() { return frame_buffer.data(); }
        SDL_PixelFormat getPixelFormat() { return pixelFormat; }

      private:
        bool fullScreen{false};
        int posX{0};
        int posY{0};

        std::size_t width{800};
        std::size_t height{600};

        std::vector<uint32_t> frame_buffer;

        SDL_PixelFormat pixelFormat;
        SDL_Texture* texture{nullptr};
        SDL_Renderer* renderer{nullptr};
        SDL_Window* window{nullptr};

        std::string title;
    };
} // namespace ce
