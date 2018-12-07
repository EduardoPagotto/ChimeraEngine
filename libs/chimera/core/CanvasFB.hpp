
#ifndef __CHIMERA_VIDEO_DEVICE__HPP
#define __CHIMERA_VIDEO_DEVICE__HPP

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

    Uint32* getPixels() { return pixels; }

  private:
    Uint32* pixels;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
};
} // namespace Chimera

#endif
