#ifndef CAMERA_H_
#define CAMERA_H_

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <tinyxml2.h>

#include "Node.h"

namespace Chimera
{

enum class CameraType
{
    Base,
    Spherical,
    Land,
    Air,
    Ovr
};

class Camera : public Node
{
public:
    Camera ( CameraType _type, std::string _id, std::string _name );
    Camera ( const Camera& _camera );

    virtual ~Camera();
    virtual void update ( DataMsg *_dataMsg );
    virtual void init();
    virtual void clone ( Node **ppNode );

    void exec();

    void setType ( CameraType _type ) {
        type = _type;
    }

    void setNear ( float _near ) {
        this->nearDistance = _near;
    }

    void setFar ( float _far ) {
        this->farDistance = _far;
    }

    void setFov ( float _fov ) {
        this->fov = _fov;
    }

    float getNear() const {
        return nearDistance;
    }

    float getFar() const {
        return farDistance;
    }

    float getFov() const {
        return fov;
    }

    bool isPerspective() {
        return perspective;
    }

    btVector3 getPosition() const {
        return position;
    }

    void setPerspective ( bool _perspective ) {
        perspective = _perspective; //TODO implementar isto para ortogonal (false);
    }

    void setTransform ( const btTransform &_trans ) {
        transform = _trans;
    }

    void setPositionRotation ( const btVector3 &_posicao, const btVector3 &_rotation );
    void loadCollada ( tinyxml2::XMLElement* _nNode );

protected:
    CameraType type;
    btVector3 position;
    btVector3 rotation;
    btVector3 direction;
    btTransform transform;

private:
    float nearDistance;
    float farDistance;
    float fov;
    bool perspective;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
