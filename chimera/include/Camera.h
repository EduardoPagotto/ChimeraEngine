#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/gl.h>          // Header File For The OpenGL32 Library
#include <GL/glu.h>         // Header File For The GLu32 Library
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Node.h"
#include "Transform.h"

namespace Chimera {

enum class CameraType { 
    LAND_CAM, 
    AIR_CAM 
};    

class Camera : public Node {
public:
    Camera (CameraType _camType, std::string id, std::string nome);
    //Camera (const Camera& camera);
    virtual ~Camera();

    virtual void update ( DataMsg *dataMsg );

    void exec ( void );
   
    virtual void init ( void );
    
    void setNear(float near) {
        this->near = near;
    }
    
    void setFar(float far) {
        this->far = far;
    }
    
    void setFov(float fov) {
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
