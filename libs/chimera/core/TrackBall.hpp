#ifndef __CHIMERA_TRACK_BALL__HPP
#define __CHIMERA_TRACK_BALL__HPP

#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

class TrackBall {

  public:
    TrackBall();
    ~TrackBall();

    virtual void init(ViewPoint* _vp);

    void tracking(int _mx, int _my, int _mz);

    void setDistanciaMaxima(const float& _distanciaMaxima) { distanciaMax = _distanciaMaxima; }
    void setDistanciaMinima(const float& _distanciaMinima) { distanciaMin = _distanciaMinima; }

  private:
    float horizontal;
    float vertical;
    float distancia;
    float distanciaMax;
    float distanciaMin;
    ViewPoint* pVp;
};
} // namespace Chimera
#endif