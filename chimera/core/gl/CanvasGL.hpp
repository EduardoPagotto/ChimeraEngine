#pragma once
#include "chimera/base/interfaces.hpp"
#include <SDL3/SDL.h>

#define GLEW_STATIC

namespace ce {

    class CanvasGL : public ServiceBase<ICanva> {

      protected:
        std::string title;
        int width, height;
        bool fullScreen{false};
        int posX, posY;
        SDL_Window* window{nullptr};
        SDL_GLContext context;

      public:
        CanvasGL(const std::string& title, int width, int height, bool fullScreen = false);

        virtual ~CanvasGL();

        virtual void before() override;

        virtual void after() override;

        virtual void toggleFullScreen() override;

        virtual void reshape(int _width, int _height) override;

        virtual const int getWidth() const override { return width; }

        virtual const int getHeight() const override { return height; }

        // FB only
        virtual uint32_t* getPixels() override { return nullptr; }
        SDL_PixelFormat getPixelFormat() override { return SDL_PIXELFORMAT_ABGR8888; }
    };
} // namespace ce
