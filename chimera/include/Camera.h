#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/gl.h>          // Header File For The OpenGL32 Library
#include <GL/glu.h>         // Header File For The GLu32 Library
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Transform.h"

namespace Chimera {

enum class CameraType { 
    LAND_CAM, 
    AIR_CAM 
};    

class Camera : public Transform {
public:
    Camera ( std::string name , CameraType _camType);
    virtual ~Camera();

    virtual void update ( DataMsg *dataMsg );

    void exec ( void );
   
    virtual void init ( void );
    
    void set(float near, float far, float fov) {
        this->near = near;
        this->far = far;
        this->fov = fov;
    }
    
    float getNear() const {
        return near;
    }
    
    float getFar() const {
        return far;
    }
    
    float getFov() const {
        return fov;
    }

protected:    
    
    CameraType cameraType;
    
private:

    

    float near;
    float far;
    float fov;
    
    

};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
