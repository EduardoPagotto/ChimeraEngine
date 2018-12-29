#include "chimera/core/TrackBall.hpp"

namespace Chimera {

TrackBall::TrackBall() {
    // yaw = 180.0f;
    // pitch = -90.0f;
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

    updateVectors();
}

void TrackBall::updateVectors() {

    float theta = glm::radians(yaw); // yaw * 0.017453293f;
    float phi = glm::radians(pitch); // pitch * 0.017453293f;
    if (pVp->up.y == 1) {
        pVp->position.x = distancia * sin(phi) * sin(theta);
        pVp->position.y = distancia * cos(phi);
        pVp->position.z = distancia * sin(phi) * cos(theta);
    } else { // pVp->up.z == 1 ou -1
        pVp->position.x = distancia * cos(theta) * sin(phi);
        pVp->position.y = distancia * cos(theta) * cos(phi);
        pVp->position.z = distancia * sin(theta);
    }
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

    if (pVp->up.y == 1) {

        yaw -= _mx;
        pitch -= _my;
        if (pitch < 1.0f)
            pitch = 1.0f;
        if (pitch > 179.0f)
            pitch = 179.0f;

    } else { // pVp->up.z == 1 ou -1

        yaw += _my;
        pitch += _mx;
        // if (yaw < 1.0f)
        //     yaw = 1.0f;
        // if (yaw > 179.0f)
        //     yaw = 179.0f;
    }

    this->updateVectors();
}
} // namespace Chimera
