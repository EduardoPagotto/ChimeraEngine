#include "chimera/core/TrackHead.hpp"

namespace Chimera {

TrackHead::TrackHead() {
    Yaw = -90.0f;
    Pitch = 0.0;
    MouseSensitivity = 1.0;
    pVp = nullptr;
}

TrackHead::~TrackHead() {}

void TrackHead::init(ViewPoint* _vp) {
    pVp = _vp;
    updateVectors();
}

void TrackHead::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    pVp->front = glm::normalize(front);
}

void TrackHead::tracking(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateVectors();
}
} // namespace Chimera
