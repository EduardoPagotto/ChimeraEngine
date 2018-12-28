#include "chimera/core/TrackBall.hpp"

namespace Chimera {

TrackBall::TrackBall() {
    Yaw = 0.0;
    Pitch = 0.0;
    MouseSensitivity = 1.0;
    pVp = nullptr;
}

TrackBall::~TrackBall() {}

void TrackBall::init(ViewPoint* _vp) {
    pVp = _vp;
    updateVectors();
}

void TrackBall::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    pVp->front = glm::normalize(front);
}

void TrackBall::tracking(float xoffset, float yoffset, bool constrainPitch) {
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

// void TrackBall::tracking(int _mx, int _my, int _mz) {

//     horizontal += _mx;
//     vertical += _my;
//     if (_mz) {

//         float l_distTemp = distancia;
//         l_distTemp += _mz;
//         if ((l_distTemp > distanciaMin) && (l_distTemp < distanciaMax)) {
//             distancia = l_distTemp;
//         }
//     }

//     float l_kx = horizontal * 0.017453293f;
//     float l_ky = vertical * 0.017453293f;

//     pVp->position.x = distancia * cos(l_kx) * sin(l_ky);
//     pVp->position.y = distancia * cos(l_kx) * cos(l_ky);
//     pVp->position.z = distancia * sin(l_kx);
// }
} // namespace Chimera
