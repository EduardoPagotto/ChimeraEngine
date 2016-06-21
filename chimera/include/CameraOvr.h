#ifndef CAMERA_OVR_H_
#define CAMERA_OVR_H_

#include "Camera.h"

namespace Chimera{
namespace Graph {
    
class CameraOvr : public Camera
{
public:
    CameraOvr ( std::string _id, std::string _name );
    CameraOvr ( const CameraOvr& _cameraOvr );
    CameraOvr ( const Camera& _camera );
    virtual ~CameraOvr();
};
}
}
#endif