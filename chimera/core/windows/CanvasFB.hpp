
#ifndef __CHIMERA_CANVAS_FB__HPP
#define __CHIMERA_CANVAS_FB__HPP

#include "Canvas.hpp"

namespace Chimera {

class CanvasFB : public Canvas {
  public:
    CanvasFB(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasFB();
    virtual void before(const unsigned short& _indexEye = 0);
    virtual void after(const unsigned short& _indexEye = 0);
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;

    // FIXME:  ver depois!!!!
    virtual void swapWindow() override{};
    virtual int getTotEyes() override { return 1; }
    uint32_t* getPixels() override { return pixels; }
    uint32_t getPixelFormat() { return pixelFormat; }

  private:
    uint32_t* pixels;
    uint32_t pixelFormat;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
};
} // namespace Chimera

#endif
