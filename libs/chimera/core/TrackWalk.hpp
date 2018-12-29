#ifndef __CHIMERA_TRACK_WALK__HPP
#define __CHIMERA_TRACK_WALK__HPP

#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

// enum class WalkType { Land, Air };
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

class TrackWalk {
  public:
    TrackWalk();
    virtual ~TrackWalk();
    void init(ViewPoint* _vp);
    void tracking(Camera_Movement direction, float deltaTime);
    glm::mat4 getViewMatrix() { return glm::lookAt(pVp->position, pVp->position + pVp->front, pVp->up); }

  private:
    void updateVectors();
    glm::vec3 right;
    float movementSpeed;
    ViewPoint* pVp;
};
} // namespace Chimera
#endif