#ifndef TRACK_CAM_H_
#define TRACK_CAM_H_

#include "Camera.h"

namespace Chimera {

class TrackCam : public Camera {
    
public:
    TrackCam(std::string name, CameraType _camType);
    ~TrackCam();
    
    virtual void init ( void );
    virtual void update ( DataMsg *dataMsg );
    
    void Pitch ( GLfloat theta );
    void Yaw ( GLfloat theta );
    void Roll ( GLfloat theta );
    
    void Walk ( GLfloat delta, bool Wall[4] );
    void Strafe ( GLfloat delta, bool Wall[4] );
    void Fly ( GLfloat delta, bool Wall[4] );
    
    void trackBall ( int _mx, int _my, int _mz );
    
    void reset();
    void updateTrack();
    
private:
    
    void initTrackBall ( void );
    
    btVector3 along;
    btVector3 up;
    btVector3 forward;
    
    float horizontal;
    float vertical;
    float distancia;
    float distanciaMax;
    float distanciaMin;
};

}
#endif