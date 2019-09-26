#include "chimera/core/TrackHead.hpp"

namespace Chimera {

TrackHead::TrackHead() {
    yaw = -90.0f;
    pitch = 0.0;
    rotSen = 0.1f;
    movementSpeed = 2.5f;
    pVp = nullptr;
}

TrackHead::~TrackHead() {}

void TrackHead::init(ViewPoint* _vp) {
    pVp = _vp;
    updateVectors();
}

void TrackHead::updateVectors() {

    // Update Rotation
    // Calculates the front vector from the Camera's (updated) Euler Angles
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    pVp->front = glm::normalize(front);

    // Update Move
    // Also re-calculate the Right and Up vector
    // Normalize the vectors, because their length gets closer to 0 the
    // more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(pVp->front, pVp->worldUp));
    pVp->up = glm::normalize(glm::cross(right, pVp->front));
}

void TrackHead::move(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        pVp->position += pVp->front * velocity;

    if (direction == BACKWARD)
        pVp->position -= pVp->front * velocity;

    if (direction == LEFT) {
        pVp->position -= right * velocity;
        // pVp->front -= right * velocity;
    }

    if (direction == RIGHT) {
        pVp->position += right * velocity;
        // pVp->front += right * velocity;
    }

    this->updateVectors();
}

void TrackHead::rotation(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= rotSen;
    yoffset *= rotSen;
    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    this->updateVectors();
}
} // namespace Chimera
