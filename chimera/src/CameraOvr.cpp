#include "CameraOvr.h"

namespace Chimera {
namespace Graph {
    
CameraOvr::CameraOvr ( std::string _name ) : Camera ( CameraType::Ovr, _name ) {

    //along.setValue ( 1.0f, 0.0f, 0.0f );
    //up.setValue ( 0.0f, 1.0f, 0.0f );
    //forward.setValue ( 0.0f,  0.0f, -1.0f );

}

CameraOvr::CameraOvr ( const CameraOvr& _cameraOvr ) : Camera ( _cameraOvr ) {

    //along = _cameraMan.along;
    //up = _cameraMan.up;
    //forward = _cameraMan.forward;

}

CameraOvr::CameraOvr ( const Camera& _camera ) : Camera ( _camera ) {

    type = CameraType::Spherical;

    //along.setValue ( 1.0f, 0.0f, 0.0f );
    //up.setValue ( 0.0f, 1.0f, 0.0f );
    //forward.setValue ( 0.0f,  0.0f, -1.0f );

}

CameraOvr::~CameraOvr() {
}
}
}
