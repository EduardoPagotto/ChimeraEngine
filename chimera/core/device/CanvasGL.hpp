#pragma once
#include "interfaces.hpp"
#include <SDL2/SDL.h>

#define GLEW_STATIC

namespace Chimera {
class CanvasGL : public ServiceBase<ICanva> {
  protected:
    bool fullScreen{false};
    int width, height;
    int posX, posY;
    std::string title;
    SDL_Window* window{nullptr};
    SDL_GLContext context;

  public:
    CanvasGL(const std::string& _title, int _width, int _height, bool _fullScreen = false);
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
} // namespace Chimera
