#ifndef VIDEO_DEVICE_H_
#define VIDEO_DEVICE_H_

#include "Video.h"

namespace Chimera {
    
class VideoDevice : public Video
{
public:
    VideoDevice ( std::string _nome );
    VideoDevice ( int _width, int _height, std::string _nome );
    virtual ~VideoDevice();

    virtual void initDraw();
    virtual void endDraw();
    virtual void executeViewPerspective ( const float &_fov,const float &_near,const float &_far, int _eye );
    
    //TODO subistituir o executeViewPerspective
    virtual void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );
    
    virtual void executeViewOrto ( int eye );
    virtual void reshape ( int _w, int _h );
    virtual void toggleFullScreen();

private:
    bool fullscreenStatus;
    SDL_Point winPosPrev;
};
}

#endif