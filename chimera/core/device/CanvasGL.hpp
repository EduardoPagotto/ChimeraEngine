#pragma once
#include "chimera/core/device/Canvas.hpp"

#define GLEW_STATIC

namespace Chimera {

class CanvasGL : public Canvas {
  protected:
    SDL_GLContext context;

  public:
    CanvasGL(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasGL();

    virtual void before() override;
    virtual void after() override;
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;
    virtual uint32_t* getPixels() override { return nullptr; }

    std::string getVersaoOpenGL();
};
} // namespace Chimera
