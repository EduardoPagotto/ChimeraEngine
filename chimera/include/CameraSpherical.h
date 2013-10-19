#ifndef TRACK_CAM_H_
#define TRACK_CAM_H_

#include "Camera.h"

namespace Chimera {

class CameraSpherical : public Camera {
    
public:
    CameraSpherical();
    
    CameraSpherical(const CameraSpherical& _cameraSpherical);
    
    CameraSpherical ( const Camera& _camera );
    
    ~CameraSpherical();
    
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
    
    void setDistanciaMaxima(btScalar _distanciaMaxima) {
        distanciaMax = _distanciaMaxima;
    }

    void setDistanciaMinima(btScalar _distanciaMinima) {
        distanciaMin = _distanciaMinima;
    }
    
private:
    
    void initTrackBall ();
    
    btVector3 along;
    btVector3 up;
    btVector3 forward;
    
    btScalar horizontal;
    btScalar vertical;
    btScalar distancia;
    btScalar distanciaMax;
    btScalar distanciaMin;
};

}
#endif