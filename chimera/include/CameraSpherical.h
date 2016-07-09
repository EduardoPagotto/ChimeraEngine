#ifndef CAMERA_SPHERICAL_H_
#define CAMERA_SPHERICAL_H_

#include "Camera.h"

namespace Chimera {

class CameraSpherical : public Camera
{

public:
    CameraSpherical (std::string _name );
    CameraSpherical ( const Camera& _camera );
    ~CameraSpherical();

	virtual void init();

	virtual void render();

    void trackBall ( int _mx, int _my, int _mz );

    void setDistanciaMaxima ( btScalar _distanciaMaxima ) {
        distanciaMax = _distanciaMaxima;
    }

    void setDistanciaMinima ( btScalar _distanciaMinima ) {
        distanciaMin = _distanciaMinima;
    }

private:

    void initTrackBall();

    btScalar horizontal;
    btScalar vertical;
    btScalar distancia;
    btScalar distanciaMax;
    btScalar distanciaMin;
};
}
#endif
