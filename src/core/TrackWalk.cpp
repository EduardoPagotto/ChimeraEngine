#include "chimera/core/TrackWalk.hpp"

namespace Chimera {

TrackWalk::TrackWalk() { pVp = nullptr; }

TrackWalk::~TrackWalk() {}

void TrackWalk::init(ViewPoint* _vp) {
    pVp = _vp;
    updateVectors();
}

void TrackWalk::tracking(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        pVp->position += pVp->front * velocity;
    if (direction == BACKWARD)
        pVp->position -= pVp->front * velocity;
    if (direction == LEFT)
        pVp->position -= right * velocity;
    if (direction == RIGHT)
        pVp->position += right * velocity;
}

void TrackWalk::updateVectors() {
    // Normalize the vectors, because their length gets closer to 0 the
    // more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(pVp->front, pVp->worldUp));
    pVp->up = glm::normalize(glm::cross(right, pVp->front));
}
} // namespace Chimera
