#ifndef _SOLID_H__
#define _SOLID_H__

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletCollision/CollisionShapes/btMaterial.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

#include "PhysicsControl.h"

#include "Coord.h"

#include <tinyxml2.h>

#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

class SolidMaterial : Entity {
public:
    SolidMaterial(std::string _name) : Entity(EntityKind::SOLID_MATERIAL, _name) {
        frictionDynamic = 10.0;
        frictionStatic = 0.0;
        restitution = 0.0;
    }

    SolidMaterial(const SolidMaterial& _solidMaterial) : Entity(_solidMaterial) {
        frictionDynamic = _solidMaterial.frictionDynamic;
        frictionStatic = _solidMaterial.frictionStatic;
        restitution = _solidMaterial.restitution;   
    }

    virtual ~SolidMaterial(){}

    inline void setFrictionDynamic ( const float &_friction ) {
        frictionDynamic = _friction;
    }

    inline void setFrictionStatic ( const float &_friction ) {
        frictionStatic = _friction;
    }

    inline void setRestitution ( const float &_restitution ) {
        restitution = _restitution;
    }

    inline btScalar getFrictionDynamic() const {
        return frictionDynamic;
    }

    inline btScalar getFrictionStatic() const {
        return frictionStatic;
    }

    inline btScalar getRestitution() const {
        return restitution;
    }

private:
    btScalar frictionDynamic;
    btScalar frictionStatic;
    btScalar restitution;
};

class Solid : public Coord
{
public:

    friend class Loader;

    Solid (Node *_parent, std::string _name);

    Solid ( const Solid& _solid );

    virtual ~Solid();

	virtual void init();

	glm::mat4 getModelMatrix(Coord *_pCoord);

    // Inherited via Node
	void accept(class NodeVisitor* v);

    // Inherited via Coord
	virtual glm::vec3 getPosition() override;
	virtual void setPosition(const glm::vec3 &_pos) override;
    virtual glm::vec3 getRotation() override;
    virtual void setRotation ( const glm::vec3 &_rotation ) override;
    virtual glm::mat4 getMatrix() override;
    virtual void setMatrix(const glm::mat4 &_trans) override;
	virtual void setPositionRotation(const glm::vec3 & _posicao, const glm::vec3 & _rotation) override;

    inline void setMass ( const float &_mass ) {
        mass = _mass;
    }

    inline void setShapeBox ( const glm::vec3 &_size ) {
        pShapeCollision = new btBoxShape ( btVector3( _size.x, _size.y, _size.z ));
    }

    inline void setShapeCilinder ( const glm::vec3 &_val ) {
        pShapeCollision = new btCylinderShape ( btVector3(_val.x, _val.y, _val.z) );
    }

    inline void setShapePlane ( const glm::vec3 &_val, const float &_constant ) {
        pShapeCollision = new btStaticPlaneShape ( btVector3( _val.x, _val.y, _val.z) , _constant );
    }

    inline void setShapeSphere ( float _raio ) {
        pShapeCollision = new btSphereShape ( (btScalar)_raio );
    }
    
    void applyForce( const glm::vec3 &_prop );
    void applyTorc( const glm::vec3 &_torque );

    void setIndexVertexArray ( btTriangleIndexVertexArray *_indexVertexArray );

    bool isShapeDefine() {
		return (pShapeCollision != nullptr ? true : false);
    }

    void setSolidMaterial(SolidMaterial _solidMaterial) {
        if (pSolidMaterial != nullptr)
            delete pSolidMaterial;

        pSolidMaterial = new SolidMaterial(_solidMaterial);
    }

    //inline void setFriction ( const float &_friction ) {
    //    friction = _friction;
    //}

    //inline void setRestitution ( const float &_restitution ) {
    //    restitution = _restitution;
    //}

// 	void setTransform(const glm::mat4 &_trans) {
//         transform.setFromOpenGLMatrix((btScalar*)glm::value_ptr(_trans));
// 	}

private:
    
    void initTransform (const btTransform &_tTrans, void *pObj );

    btScalar mass;

    //btScalar friction;
    //btScalar restitution;

    SolidMaterial *pSolidMaterial;

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
