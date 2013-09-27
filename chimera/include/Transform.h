#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <LinearMath/btVector3.h>
#include "Node.h"

namespace Chimera {

class Transform : public Node {
public:
    Transform ( EntityType _type, std::string _name ) : Node ( _type,_name ) {
        scale.setValue ( 0.0,0.0,0.0 );
        position.setZero();
        direction.setZero();
        rotation.setZero();
    }

    Transform() : Node ( EntityType::TRANSFORM,"" ) {
        scale.setValue ( 0.0,0.0,0.0 );
        position.setZero();
        direction.setZero();
        rotation.setZero();
    }

    virtual ~Transform();

    inline btVector3 getScale() const {
        return scale;
    }

    inline btVector3 getPosition() const {
        return position;
    }

    inline btVector3 getDirection() const {
        return direction;
    }

    inline btVector3 getRotation() const {
        return rotation;
    }

    inline void setScale ( btVector3 scale ) {
        this->scale = scale;
    }

    inline void setPosition ( btVector3 position ) {
        this->position = position;
    }

    inline void setDirection ( btVector3 direction ) {
        this->direction = direction;
    }

    inline void setRotation ( btVector3 rotation ) {
        this->rotation = rotation;
    }


protected:
    btVector3 scale;
    btVector3 position;
    btVector3 direction;
    btVector3 rotation;
};




}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
