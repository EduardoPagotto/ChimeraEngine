#include "chimera/core/TrackBall.hpp"

namespace Chimera {

TrackBall::TrackBall() {
    horizontal = 0.0f;
    vertical = 0.0f;
    distancia = 0;
    distanciaMin = 1.0f;
    distanciaMax = 500.0f;
    pVp = nullptr;
}

TrackBall::~TrackBall() {}

void TrackBall::init(ViewPoint* _vp) {
    pVp = _vp;
    distancia = glm::distance(pVp->position, pVp->direction);
    vertical = asin((glm::abs(pVp->position.z) - glm::abs(pVp->direction.z)) / distancia) / 0.017453293f;
    horizontal = asin((glm::abs(pVp->position.y) - glm::abs(pVp->direction.y)) / distancia) / 0.017453293f;
}

void TrackBall::tracking(int _mx, int _my, int _mz) {

    horizontal += _mx;
    vertical += _my;
    if (_mz) {

        float l_distTemp = distancia;
        l_distTemp += _mz;
        if ((l_distTemp > distanciaMin) && (l_distTemp < distanciaMax)) {
            distancia = l_distTemp;
        }
    }

    float l_kx = horizontal * 0.017453293f;
    float l_ky = vertical * 0.017453293f;

    pVp->position.x = distancia * cos(l_kx) * sin(l_ky);
    pVp->position.y = distancia * cos(l_kx) * cos(l_ky);
    pVp->position.z = distancia * sin(l_kx);
}
} // namespace Chimera
