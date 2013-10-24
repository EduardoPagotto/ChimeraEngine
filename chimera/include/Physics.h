#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletCollision/CollisionShapes/btMaterial.h>

#include "PhysicsControl.h"
#include "Singleton.h"


namespace Chimera {

    class PhysicsControl;
    
class Physics {
public:
    friend class Loader;

    Physics();
    ~Physics();

    inline void setMass (const float &_mass ) { mass = _mass; }
    
    inline void setFriction (const float &_friction ) { friction = _friction; }    
    
    inline void setRestitution (const float &_restitution ) { restitution = _restitution; }
    
    inline void setShapeBox ( const btVector3 &_size ) { pShapeCollision = new btBoxShape ( _size );}
    
    inline void setShapeCilinder(const btVector3 &_val) { pShapeCollision = new btCylinderShape( _val ); }

    inline btVector3& getPosition() { return pRigidBody->getWorldTransform().getOrigin();}
    
    void init (btTransform &_tTrans, void *pObj);

    //usada na trans cam do mundo
    void transformacao3D ();

    //usada na trans da cam objeto
    void ajusteMatrix ( Physics *_pPhysic );

    void propulcao ( const btVector3 &_prop );
    void torque ( const btVector3 &_torque );
    
private:
    btScalar mass;
    btScalar friction;
    btScalar restitution;
    
    btRigidBody* pRigidBody;
    btCollisionShape* pShapeCollision;
    btDefaultMotionState *pMotionState;
   
    PhysicsControl *pWorld;
    
    //btTriangleIndexVertexArray *m_pIndexVertexArrays;
};

}
#endif //Physics_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
