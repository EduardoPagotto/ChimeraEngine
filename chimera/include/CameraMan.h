#ifndef CAMERA_MAN_H_
#define CAMERA_MAN_H_

#include "Camera.h"

namespace Chimera {

class CameraMan : public Camera {

public:
    CameraMan();

    CameraMan ( const CameraMan& _cameraMan );

    CameraMan ( const Camera& _camera );

    ~CameraMan();

    virtual void update ( DataMsg *_dataMsg );

    void pitch ( GLfloat _theta );
    void yaw ( GLfloat _theta );
    void roll ( GLfloat _theta );

    void walk ( GLfloat _delta, bool _wall[4] );
    void strafe ( GLfloat _delta, bool _wall[4] );
    void fly ( GLfloat _delta, bool _wall[4] );

    void reset();
    void updateMove();

private:
    btVector3 along;
    btVector3 up;
    btVector3 forward;

};

}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
