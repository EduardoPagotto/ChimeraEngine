#ifndef CAMERA_MAN_H_
#define CAMERA_MAN_H_

#include "Camera.h"

namespace Chimera
{

class CameraMan : public Camera
{

public:
    CameraMan ( std::string _id, std::string _name );
    CameraMan ( const CameraMan& _cameraMan );
    CameraMan ( const Camera& _camera );
    virtual ~CameraMan();
    
    virtual void update ( DataMsg *_dataMsg );
    virtual void clone ( Node **ppNode );

    void pitch ( float _theta );
    void yaw ( float _theta );
    void roll ( float _theta );
    void walk ( float _delta, bool _wall[4] );
    void strafe ( float _delta, bool _wall[4] );
    void fly ( float _delta, bool _wall[4] );
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
