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
    
class Solid : public Coord
{
public:

    friend class Loader;

    Solid (Node *_parent, std::string _name);

    Solid ( const Solid& _solid );

    virtual ~Solid();

    virtual void apply(Coord *_pCoord);

	virtual void init();

    // Inherited via Node
	void accept(class NodeVisitor* v);

	virtual glm::vec3 getPosition() {
		btVector3 pos = pRigidBody->getWorldTransform().getOrigin();
        return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
	}

	virtual void setPosition(const glm::vec3 &_pos);

	void setPositionRotation(const glm::vec3 & _posicao, const glm::vec3 & _rotation);

    inline void setMass ( const float &_mass ) {
        mass = _mass;
    }

    inline void setFriction ( const float &_friction ) {
        friction = _friction;
    }

    inline void setRestitution ( const float &_restitution ) {
        restitution = _restitution;
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

    void setRotation ( const glm::vec3 &_rotation );

    glm::vec3 getRotation();

    //usada na trans cam do mundo
    void transformacao3D();
    
    void applyForce( const glm::vec3 &_prop );
    void applyTorc( const glm::vec3 &_torque );

    void setIndexVertexArray ( btTriangleIndexVertexArray *_indexVertexArray );

    bool isShapeDefine() {
		return (pShapeCollision != nullptr ? true : false);
    }

	void setTransform(const glm::mat4 &_trans) {
                
		transform.getOpenGLMatrix((btScalar*)glm::value_ptr(_trans));//TODO TESTAR!!!!!
	}

    void loadColladaPhysicsModel ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nRigidBody, std::string &_meshName );

private:
    
    void initTransform (const btTransform &_tTrans, void *pObj );
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
