#ifndef __CHIMERA_VIDEO_DEVICE__HPP
#define __CHIMERA_VIDEO_DEVICE__HPP

#include "Video.hpp"

namespace Chimera {

class VideoDevice : public Video {
  public:
    VideoDevice(std::string _nome);
    VideoDevice(int _width, int _height, std::string _nome);
    virtual ~VideoDevice();

    virtual void initDraw();
    virtual void endDraw();
    virtual glm::mat4 getPerspectiveProjectionMatrix(const float& _fov,
                                                     const float& _near,
                                                     const float& _far, int _eye);

    // TODO subistituir o executeViewPerspective
    // virtual void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear,
    // GLdouble zFar );

    virtual int getTotEyes() { return 1; }
    // virtual void executeViewOrto ( int eye );
    virtual glm::mat4 getOrthoProjectionMatrix(int eyeIndex);

    virtual void reshape(int _w, int _h);
    virtual void toggleFullScreen();

  private:
    bool fullscreenStatus;
    SDL_Point winPosPrev;
};
} // namespace Chimera

#endif
