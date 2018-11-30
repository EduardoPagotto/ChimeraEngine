#include "CameraOvr.hpp"

namespace Chimera {

CameraOvr::CameraOvr(std::string _name) : Camera(nullptr, CameraType::Ovr, _name) {

    // along.setValue ( 1.0f, 0.0f, 0.0f );
    // up.setValue ( 0.0f, 1.0f, 0.0f );
    // forward.setValue ( 0.0f,  0.0f, -1.0f );
}

CameraOvr::CameraOvr(const Camera& _camera) : Camera(_camera) {

    type = CameraType::Spherical;

    // along.setValue ( 1.0f, 0.0f, 0.0f );
    // up.setValue ( 0.0f, 1.0f, 0.0f );
    // forward.setValue ( 0.0f,  0.0f, -1.0f );
}

CameraOvr::~CameraOvr() {}

void CameraOvr::render() {}

void CameraOvr::init() {}
} // namespace Chimera
