#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include "Node.h"

namespace Chimera {

class Transform : public Node {
public:
    Transform ( ) : Node ( EntityType::TRANSFORM) {
        scale.setValue ( 0.0,0.0,0.0 );
        position.setZero();
        direction.setZero();
        rotation.setZero();
    }

//     Transform ( const Transform& transform ) : Node ( transform ) {
//         
//         scale = transform.scale;
//         position = transform.position;
//         direction = transform.direction;
//         rotation = transform.rotation;
//         
//     }
    
//     Transform() : Node ( EntityType::TRANSFORM,"" ) {
//         scale.setValue ( 0.0,0.0,0.0 );
//         position.setZero();
//         direction.setZero();
//         rotation.setZero();
//     }

    virtual ~Transform();

    inline btScalar distanciaPosicaoDirecao() const {
        return position.distance(direction);
    }

    inline btScalar distanciaPosicaoDirecaoZ() const {
        return asin ( ( btFabs ( position.z() ) - btFabs ( direction.z() ) ) / position.distance(direction) ) / 0.017453293f;
    }

    inline btScalar distanciaPosicaoDirecaoY() const {
        return asin ( ( btFabs ( position.y() ) - btFabs ( direction.y() ) ) / position.distance(direction) ) / 0.017453293f;
    }
    
    inline btScalar distanciaPosicaoDirecaoX() const {
        return asin ( ( btFabs ( position.x() ) - btFabs ( direction.x() ) ) / position.distance(direction) ) / 0.017453293f;
    }
    
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

    inline void setOpenGLMatrix(const btScalar *m) {
        transform.setFromOpenGLMatrix(m);
    }
    
    
protected:
    btVector3 scale;
    btVector3 position;
    btVector3 direction;
    btVector3 rotation;
    
    btTransform transform;
};




}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
