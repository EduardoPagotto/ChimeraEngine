#ifndef __CHIMERA_TRACK_BALL__HPP
#define __CHIMERA_TRACK_BALL__HPP

#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

class TrackBall {

  public:
    TrackBall();
    virtual ~TrackBall();

    void init(ViewPoint* _vp);
    void tracking(float xoffset, float yoffset, bool constrainPitch = true);

    ViewPoint* getViewPoint() { return pVp; }

  private:
    void updateVectors();
    float Yaw;
    float Pitch;
    float MouseSensitivity;
    ViewPoint* pVp;
};
} // namespace Chimera
#endif