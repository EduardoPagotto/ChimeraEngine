#ifndef __CHIMERA_TRACK_HEAD__HPP
#define __CHIMERA_TRACK_HEAD__HPP

#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

class TrackHead {

  public:
    TrackHead();
    virtual ~TrackHead();

    void init(ViewPoint* _vp);
    void rotation(float xoffset, float yoffset, bool constrainPitch = true);
    void move(Camera_Movement direction, float deltaTime);

    glm::mat4 getViewMatrix() { return glm::lookAt(pVp->position, pVp->position + pVp->front, pVp->up); }

    inline ViewPoint* getViewPoint() { return pVp; }

  private:
    void updateVectors();

    // rot
    float yaw;
    float pitch;
    float rotSen;

    // move
    glm::vec3 right;
    float movementSpeed;

    ViewPoint* pVp;
};
} // namespace Chimera
#endif