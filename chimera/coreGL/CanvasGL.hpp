#pragma once
#include "core/interfaces.hpp"
#include <SDL2/SDL.h>

#define GLEW_STATIC

namespace ce {
class CanvasGL : public ServiceBase<ICanva> {
  protected:
    std::string title;
    int width, height;
    int posX, posY;
    bool fullScreen{false};

    SDL_Window* window{nullptr};
    SDL_GLContext context;

  public:
    CanvasGL(const std::string& title, int width, int height, bool fullScreen = false);
    virtual ~CanvasGL();

    virtual void before() override;
    virtual void after() override;
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;
    virtual uint32_t* getPixels() override { return nullptr; }
    virtual const int getWidth() const override { return width; }
    virtual const int getHeight() const override { return height; }

    std::string getVersaoOpenGL();
};
} // namespace ce
