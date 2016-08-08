#include "CameraSpherical.h"

namespace Chimera {
    
CameraSpherical::CameraSpherical ( std::string _name ) : Camera (nullptr, CameraType::Spherical, _name ) {

    horizontal = 0.0f;
    vertical = 0.0f;
    distancia = 0;
    distanciaMin = 1.0f;
    distanciaMax = 500.0f;

}

CameraSpherical::CameraSpherical ( const Camera& _camera ) : Camera ( _camera ) {

    type = CameraType::Spherical;
    horizontal = 0.0f;
    vertical = 0.0f;
    distancia = 0;
    distanciaMin = 1.0f;
    distanciaMax = 500.0f;

}

CameraSpherical::~CameraSpherical() {
}

void CameraSpherical::init() {

	Camera::init();

	initTrackBall();
}

void CameraSpherical::render() {
	Camera::render();
}

void CameraSpherical::initTrackBall ( void ) {

    distancia = glm::distance(position, direction ); //position.distance ( direction );
    vertical =   asin ( ( glm::abs ( position.z ) - glm::abs ( direction.z ) ) / glm::distance(position, direction) ) / 0.017453293f;
    horizontal = asin ( ( glm::abs ( position.y ) - glm::abs ( direction.y ) ) / glm::distance(position, direction) ) / 0.017453293f;

}

void CameraSpherical::trackBall ( int _mx, int _my, int _mz ) {

    horizontal += _mx;
    vertical += _my;
    if ( _mz ) {

        float l_distTemp = distancia; //tempDistancia = distancia;
        l_distTemp += _mz;
        if ( ( l_distTemp > distanciaMin ) && ( l_distTemp < distanciaMax ) ) {
            distancia = l_distTemp;
        }

    }

    float l_kx = horizontal * 0.017453293f;
    float l_ky = vertical * 0.017453293f;

    //Transform *trans = ( Transform* ) parent;
    position.x = distancia * cos ( l_kx ) * sin ( l_ky );
    position.y = distancia * cos ( l_kx ) * cos ( l_ky );
    position.z = distancia * sin ( l_kx );

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
