#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/gl.h>          // Header File For The OpenGL32 Library
#include <GL/glu.h>         // Header File For The GLu32 Library
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Node.h"

namespace Chimera {

enum class CameraType { 
    LAND_CAM, 
    AIR_CAM 
};    

class Camera : public Node {
public:
    
    friend class Loader;
    
    Camera ();
    
    Camera (const Camera& _camera);
    
    virtual ~Camera();

    virtual void update ( DataMsg *_dataMsg );

    void exec ( void );
   
    virtual void init ( void );
    
    void setType(CameraType _type) {
        type = _type;
    }
    
    void setNear(float _near) {
        this->near = _near;
    }
    
    void setFar(float _far) {
        this->far = _far;
    }
    
    void setFov(float _fov) {
        this->fov = _fov;
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
    CameraType type;
    
    btVector3 position;
    btVector3 rotation;
    btVector3 direction;
    
    btTransform transform;
    
private:
    float near;
    float far;
    float fov;
 
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
