#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletCollision/CollisionShapes/btMaterial.h>

#include "PhysicWorld.h"
#include "Singleton.h"


namespace Chimera {

class Physics {
public:
    friend class Loader;

    Physics();
    ~Physics();

    inline void setMass ( float _mass ) {
        mass = _mass;
    }

    void setShapeBox ( const btVector3 &_size ) {
        pShapeCollision = new btBoxShape ( _size );
    }

    void init (btTransform &_tTrans, unsigned _serial);

    //usada na trans cam do mundo
    void transformacao3D ( void );

    //usada na trans da cam objeto
    void ajusteMatrix ( Physics *_pPhysic );

    void propulcao ( const btVector3 &_prop );
    void torque ( const btVector3 &_torque );

    void setPhysicMaterial(btMaterial *_pPhysicMaterial) {
        m_pPhysicMaterial = _pPhysicMaterial;
    }
    
    inline btVector3& getPosition() {
        return pRigidBody->getWorldTransform().getOrigin();
    }
    
private:

    btRigidBody* pRigidBody;

    btScalar mass;
    
    btCollisionShape* pShapeCollision;

    btTriangleIndexVertexArray *m_pIndexVertexArrays;

    btDefaultMotionState *m_pMotionState;

    PhysicWorld *m_physicWorld;

    btMaterial *m_pPhysicMaterial;
};

}
#endif //Physics_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
