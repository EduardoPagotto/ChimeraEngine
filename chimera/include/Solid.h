#ifndef _SOLID_H__
#define _SOLID_H__

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletCollision/CollisionShapes/btMaterial.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

#include "PhysicsControl.h"

#include "Coord.h"

#include <tinyxml2.h>

namespace Chimera {
    
class Solid : public Coord
{
public:

    friend class Loader;

    Solid (Node *_parent, std::string _name);

    Solid ( const Solid& _solid );

    virtual ~Solid();

    virtual void apply(Solid *_pSource);

	virtual void init();

	void accept(class NodeVisitor* v);

	virtual btVector3 getPosition() {
		return pRigidBody->getWorldTransform().getOrigin();
	}

	virtual void setPosition(const btVector3 &_pos);

	void setPositionRotation(const btVector3 & _posicao, const btVector3 & _rotation);

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

    void setRotation ( const btVector3 &_rotation );

    btVector3 getRotation();

    void initTransform ( btTransform &_tTrans, void *pObj );

    //usada na trans cam do mundo
    void transformacao3D();
    
    void applyForce( const btVector3 &_prop );
    void applyTorc( const btVector3 &_torque );

    void setIndexVertexArray ( btTriangleIndexVertexArray *_indexVertexArray );

    bool isShapeDefine() {
		return (pShapeCollision != nullptr ? true : false);
    }

	void setTransform(const btTransform &_trans) {
		transform = _trans;
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
	btTransform transform;
};
}
#endif //Physics_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
