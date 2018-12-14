
#ifndef __CHIMERA_CANVAS_FB__HPP
#define __CHIMERA_CANVAS_FB__HPP

#include "chimera/core/Canvas.hpp"

namespace Chimera {

class CanvasFB : public Canvas {
  public:
    CanvasFB(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasFB();
    virtual void before() override;
    virtual void after() override;
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;

    uint32_t* getPixels() { return pixels; }

    uint32_t getPixelFormat() { return pixelFormat; }

  private:
    uint32_t* pixels;
    uint32_t pixelFormat;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
};
} // namespace Chimera

#endif
