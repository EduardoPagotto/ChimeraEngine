#ifndef __CHIMERA_CAMERA_OVR__HPP
#define __CHIMERA_CAMERA_OVR__HPP

#include "Camera.h"

namespace Chimera {

class CameraOvr : public Camera {
  public:
    CameraOvr(std::string _name);
    CameraOvr(const Camera& _camera);

    virtual void init();
    virtual void render();

    virtual ~CameraOvr();
};
} // namespace Chimera
#endif