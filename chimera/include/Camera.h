#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/gl.h>          // Header File For The OpenGL32 Library
#include <GL/glu.h>         // Header File For The GLu32 Library

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>

#include "Node.h"

namespace Chimera {

enum class CameraType { 
    Base,
    Spherical,
    Land,
    Air
};    

class Camera : public Node {
public:
    
    friend class Loader;
    
    Camera (CameraType _type, std::string _id, std::string _name);
    
    Camera (const Camera& _camera);
    
    virtual ~Camera();

    virtual void update ( DataMsg *_dataMsg );

    virtual void init();
    
    void exec ();
       
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

    bool isPerspective() {
        return perspective;
    }
    
    void setPerspective(bool _perspective) {
        perspective = _perspective; //TODO implementar isto para ortogonal (false);
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
 
    bool perspective;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
