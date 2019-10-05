#ifndef __CHIMERA_TRACK_BALL__HPP
#define __CHIMERA_TRACK_BALL__HPP

#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

class TrackBall {

  public:
    TrackBall();
    virtual ~TrackBall();

    void init(ViewPoint* _vp);
    void tracking(const int& _mx, const int& _my);
    void offSet(const int& _mz);

    inline void setMax(const float& _max) { max = _max; }
    inline void setMin(const float& _min) { min = _min; }
    inline ViewPoint* getViewPoint() { return pVp; }
    inline glm::mat4 getViewMatrix() { return glm::lookAt(pVp->position, pVp->front, pVp->up); }

  private:
    void updateVectors();
    float yaw;
    float pitch;
    float distancia;
    float max;
    float min;
    ViewPoint* pVp;
};
} // namespace Chimera
#endif