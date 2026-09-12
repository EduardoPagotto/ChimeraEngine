#pragma once
#include "chimera_base/ICanva.hpp"
#include <SDL3/SDL.h>
#include <string>

#define GLEW_STATIC

namespace ce {

    class CanvasGL : public ICanva {
      public:
        explicit CanvasGL(const std::string& title, int width, int height, bool fullScreen = false);
        virtual ~CanvasGL();

        virtual void before() override;
        virtual void after() override;
        virtual void toggleFullScreen() override;
        virtual void reshape(int _width, int _height) override;
        virtual uint32_t getWidth() const override { return width; }
        virtual uint32_t getHeight() const override { return height; }

      protected:
        std::string title;
        int width, height;
        bool fullScreen{false};
        int posX, posY;
        SDL_Window* window{nullptr};
        SDL_GLContext context;
    };
} // namespace ce
