#include "CameraSpherical.h"

namespace Chimera {
namespace Graph {
    
CameraSpherical::CameraSpherical ( std::string _id, std::string _name ) : Camera ( CameraType::Spherical, _id, _name ) {

    horizontal = 0.0f;
    vertical = 0.0f;
    distancia = 0;
    distanciaMin = 1.0f;
    distanciaMax = 500.0f;

}

CameraSpherical::CameraSpherical ( const CameraSpherical& _cameraSpherical ) : Camera ( _cameraSpherical ) {

    horizontal = _cameraSpherical.horizontal;
    vertical = _cameraSpherical.vertical;
    distancia = _cameraSpherical.distancia;
    distanciaMin = _cameraSpherical.distanciaMin;
    distanciaMax = _cameraSpherical.distanciaMax;

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

void CameraSpherical::clone ( Node **ppNode ) {
    *ppNode = new CameraSpherical ( *this );
    Node::clone ( ppNode );
}

void CameraSpherical::update ( DataMsg *_dataMsg ) {

    Camera::update ( _dataMsg );

    if ( _dataMsg->getKindOp() == KindOp::START ) {

        /* init(); */
        initTrackBall();

    }

}

void CameraSpherical::initTrackBall ( void ) {

    distancia = position.distance ( direction );
    vertical = asin ( ( btFabs ( position.z() ) - btFabs ( direction.z() ) ) / position.distance ( direction ) ) / 0.017453293f;
    horizontal = asin ( ( btFabs ( position.y() ) - btFabs ( direction.y() ) ) / position.distance ( direction ) ) / 0.017453293f;

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
    position.setX ( distancia * cos ( l_kx ) * sin ( l_ky ) );
    position.setY ( distancia * cos ( l_kx ) * cos ( l_ky ) );
    position.setZ ( distancia * sin ( l_kx ) );

}
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
