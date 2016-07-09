#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletCollision/CollisionShapes/btMaterial.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

#include "PhysicsControl.h"

#include <tinyxml2.h>

namespace Chimera {
    
class Physics : public Entity
{
public:

    friend class Loader;

    Physics ( std::string _name );

    Physics ( const Physics& _physics );

    ~Physics();

    inline void setMass ( const float &_mass ) {
        mass = _mass;
    }

    inline void setFriction ( const float &_friction ) {
        friction = _friction;
    }

    inline void setRestitution ( const float &_restitution ) {
        restitution = _restitution;
    }

    inline void setShapeBox ( const btVector3 &_size ) {
        pShapeCollision = new btBoxShape ( _size );
    }

    inline void setShapeCilinder ( const btVector3 &_val ) {
        pShapeCollision = new btCylinderShape ( _val );
    }

    inline void setShapePlane ( const btVector3 &_val, const float &_constant ) {
        pShapeCollision = new btStaticPlaneShape ( _val, _constant );
    }

    inline void setShapeSphere ( btScalar _raio ) {
        pShapeCollision = new btSphereShape ( _raio );
    }

    inline btVector3& getPosition() {
        return pRigidBody->getWorldTransform().getOrigin();
    }

    void setPosition ( const btVector3 &_pos );

    void setRotation ( const btVector3 &_rotation );

    btVector3 getRotation();

    void initTransform ( btTransform &_tTrans, void *pObj );
	virtual void init();

    //virtual void update ( DataMsg *_dataMsg );

    //virtual void clone ( Node **ppNode );

    //usada na trans cam do mundo
    void transformacao3D();

    //usada na trans da cam objeto
    void ajusteMatrix ( Physics *_pPhysic );

    void propulcao ( const btVector3 &_prop );
    void torque ( const btVector3 &_torque );

    void setIndexVertexArray ( btTriangleIndexVertexArray *_indexVertexArray );

    bool isShapeDefine() {

        if ( pShapeCollision != nullptr ) {
            return true;
        }

        return false;
    }

    void loadColladaPhysicsModel ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nRigidBody, std::string &_meshName );

private:

    void loadColladaShape ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, std::string &_meshName );

    btScalar mass;
    btScalar friction;
    btScalar restitution;

    btRigidBody* pRigidBody;

    btCollisionShape* pShapeCollision;
    btGImpactMeshShape *trimesh;

    btDefaultMotionState *pMotionState;

    //btTriangleIndexVertexArray *indexVertexArray;

    PhysicsControl *pWorld;

    //btTriangleIndexVertexArray *m_pIndexVertexArrays;
};
}
#endif //Physics_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
