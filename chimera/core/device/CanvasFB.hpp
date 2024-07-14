#pragma once

#include "ICanva.hpp"

namespace Chimera {

class CanvasFB : public Canvas {
  private:
    uint32_t* pixels;
    uint32_t pixelFormat;
    SDL_Texture* texture;
    SDL_Renderer* renderer;

  public:
    CanvasFB(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasFB();
    virtual void before() override;
    virtual void after() override;
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;

    uint32_t* getPixels() override { return pixels; }
    uint32_t getPixelFormat() { return pixelFormat; }
};
} // namespace Chimera
