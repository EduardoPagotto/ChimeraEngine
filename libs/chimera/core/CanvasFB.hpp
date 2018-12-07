
#ifndef __CHIMERA_VIDEO_DEVICE__HPP
#define __CHIMERA_VIDEO_DEVICE__HPP

#include "chimera/core/Canvas.hpp"

namespace Chimera {

class CanvasFrameBuffer : public Canvas {
  public:
    CanvasFrameBuffer(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasFrameBuffer();
    virtual void before() override;
    virtual void after() override;
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;

  private:
    SDL_Renderer* render;
};
} // namespace Chimera

#endif
