#ifndef __CHIMERA_TRACK_HEAD__HPP
#define __CHIMERA_TRACK_HEAD__HPP

#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

class TrackHead {

  public:
    TrackHead();
    virtual ~TrackHead();

    void init(ViewPoint* _vp);
    void tracking(float xoffset, float yoffset, bool constrainPitch = true);

    glm::mat4 getViewMatrix() { return glm::lookAt(pVp->position, pVp->position + pVp->front, pVp->up); }

    inline ViewPoint* getViewPoint() { return pVp; }

  private:
    void updateVectors();
    float Yaw;
    float Pitch;
    float MouseSensitivity;
    ViewPoint* pVp;
};
} // namespace Chimera
#endif