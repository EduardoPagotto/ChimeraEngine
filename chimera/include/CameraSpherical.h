#ifndef __CHIMERA_CAMERA_SPHERICAL__HPP
#define __CHIMERA_CAMERA_SPHERICAL__HPP

#include "Camera.h"

namespace Chimera {

class CameraSpherical : public Camera {

  public:
    CameraSpherical(std::string _name);
    CameraSpherical(const Camera& _camera);
    ~CameraSpherical();

    virtual void init();

    void trackBall(int _mx, int _my, int _mz);

    void setDistanciaMaxima(const float& _distanciaMaxima) {
        distanciaMax = _distanciaMaxima;
    }

    void setDistanciaMinima(const float& _distanciaMinima) {
        distanciaMin = _distanciaMinima;
    }

  private:
    void initTrackBall();

    float horizontal;
    float vertical;
    float distancia;
    float distanciaMax;
    float distanciaMin;
};
} // namespace Chimera
#endif
