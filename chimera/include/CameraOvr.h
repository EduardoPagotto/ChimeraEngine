#ifndef CAMERA_OVR_H_
#define CAMERA_OVR_H_

#include "Camera.h"

namespace Chimera{
    
class CameraOvr : public Camera
{
public:
    CameraOvr (std::string _name );
    CameraOvr ( const Camera& _camera );

	virtual void init();
	virtual void render();

    virtual ~CameraOvr();
};
}
#endif