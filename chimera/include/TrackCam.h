#ifndef TRACK_CAM_H_
#define TRACK_CAM_H_

#include "Camera.h"

namespace Chimera {

class TrackCam : public Camera {
    
public:
    TrackCam();
    
    TrackCam(const TrackCam& _trackCam);
    
    ~TrackCam();
    
    virtual void init ( );
    virtual void update ( DataMsg *_dataMsg );
    
    void pitch ( GLfloat _theta );
    void yaw ( GLfloat _theta );
    void roll ( GLfloat _theta );
    
    void walk ( GLfloat _delta, bool _wall[4] );
    void strafe ( GLfloat _delta, bool _wall[4] );
    void fly ( GLfloat _delta, bool _wall[4] );
    
    void trackBall ( int _mx, int _my, int _mz );
    
    void reset();
    void updateTrack();
    
private:
    
    void initTrackBall ();
    
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