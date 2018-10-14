#ifndef HMD_DEVICE_H_
#define HMD_DEVICE_H_

#include "Video.h"

namespace Chimera {

struct Canvas {
    int w,
    int h
}

struct HMD {
    Canvas resolution
}

class HmdDevice : public Video
{
public:
    HmdDevice ( std::string _nome );
    HmdDevice ( int _width, int _height, std::string _nome );
    virtual ~HmdDevice();

    virtual void initDraw();
    virtual void endDraw();
    virtual glm::mat4 getPerspectiveProjectionMatrix( const float &_fov,const float &_near,const float &_far, int _eye );
    
    //TODO subistituir o executeViewPerspective
    //virtual void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );
    
    virtual glm::mat4 getOrthoProjectionMatrix( int eyeIndex );//virtual void executeViewOrto ( int eye );
    
    virtual void reshape ( int _w, int _h );
    virtual void toggleFullScreen();

private:

    void initDevice();

    GLuint fbo, render_buf

    HMD hmd;

    Canvas fbSize
   // bool fullscreenStatus;
   // SDL_Point winPosPrev;
};
}

#endif