#include "chimera/core/TrackBall.hpp"

namespace Chimera {

TrackBall::TrackBall() {
    yaw = 0.0f;
    pitch = 0.0f;
    distancia = 0;
    min = 1.0f;
    max = 500.0f;
    pVp = nullptr;
}

TrackBall::~TrackBall() {}

void TrackBall::init(ViewPoint* _vp) {
    pVp = _vp;
    distancia = glm::distance(pVp->position, pVp->front);
    yaw = asin((glm::abs(pVp->position.z) - glm::abs(pVp->front.z)) / distancia) / 0.017453293f;
    pitch = asin((glm::abs(pVp->position.y) - glm::abs(pVp->front.y)) / distancia) / 0.017453293f;

    // updateVectors();
}

void TrackBall::updateVectors() {
    float l_kx = glm::radians(yaw);   // yaw * 0.017453293f;
    float l_ky = glm::radians(pitch); // pitch * 0.017453293f;
    pVp->position.x = distancia * cos(l_kx) * sin(l_ky);
    pVp->position.y = distancia * cos(l_kx) * cos(l_ky);
    pVp->position.z = distancia * sin(l_kx);
}

void TrackBall::offSet(const int& _mz) {
    distancia += _mz;
    if (distancia < min)
        distancia = min;
    if (distancia > max)
        distancia = max;

    this->updateVectors();
}

void TrackBall::tracking(const int& _mx, const int& _my) {

    yaw += _my;
    pitch += _mx;
    if (yaw > 89.0f)
        yaw = 89.0f;
    if (yaw < -89.0f)
        yaw = -89.0f;
    this->updateVectors();
}
} // namespace Chimera
