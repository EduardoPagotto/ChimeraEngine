#pragma once
#include "PhysicsControl.hpp"
#include "chimera/render/Transform.hpp"
#include <BulletCollision/CollisionShapes/btMaterial.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

class Solid : public ITrans {
  public:
    Solid();
    Solid(PhysicsControl* _pWorld);
    Solid(PhysicsControl* _pWorld, const glm::mat4& _trans);
    virtual ~Solid();

    // Inherited
    virtual const glm::vec3 getPosition() const override;
    virtual const glm::mat4 getMatrix() const override;
    virtual const glm::mat4 translateSrc(const glm::vec3& _pos) const override;
    // virtual const glm::vec3 getRotation() const override;
    virtual void setPosition(const glm::vec3& _pos) override;
    virtual void setRotation(const glm::vec3& _rotation) override;
    virtual void setMatrix(const glm::mat4& _trans) override;

    // prop init FIXME: melhorar!!! ainda confuso
    void init(const glm::vec3& _size);                                    // usado no scene no final da inicializacao
    void setInitParams(PhysicsControl* _pWorld, const glm::mat4& _trans); // usado no loader:LibraryPhysicModels
    // prop shape
    inline void setShapeBox(const glm::vec3& _size) { pShapeCollision = new btBoxShape(btVector3(_size.x, _size.y, _size.z)); }
    inline void setShapeCilinder(const glm::vec3& _val) { pShapeCollision = new btCylinderShape(btVector3(_val.x, _val.y, _val.z)); }
    inline void setShapePlane(const glm::vec3& _val, const float& _constant) {
        pShapeCollision = new btStaticPlaneShape(btVector3(_val.x, _val.y, _val.z), _constant);
    }
    inline void setShapeSphere(float _raio) { pShapeCollision = new btSphereShape((btScalar)_raio); }
    void setIndexVertexArray(btTriangleIndexVertexArray* _indexVertexArray);
    bool isShapeDefine() { return (pShapeCollision != nullptr ? true : false); }

    inline void setMass(const float& _mass) { mass = _mass; }

    void applyForce(const glm::vec3& _prop);
    void applyTorc(const glm::vec3& _torque);
    inline void setFrictionDynamic(const float& _friction) { frictionDynamic = _friction; }
    inline void setFrictionStatic(const float& _friction) { frictionStatic = _friction; }
    inline void setRestitution(const float& _restitution) { restitution = _restitution; }

  private:
    void initTransform(const btTransform& _tTrans, void* pObj);

    btScalar mass;

    btScalar frictionDynamic;
    btScalar frictionStatic;
    btScalar restitution;

    btRigidBody* pRigidBody;
    btCollisionShape* pShapeCollision;
    btGImpactMeshShape* trimesh;
    btDefaultMotionState* pMotionState;
    // btTriangleIndexVertexArray *indexVertexArray;
    PhysicsControl* pWorld;
    // btTriangleIndexVertexArray *m_pIndexVertexArrays;
    btTransform transform;
};
} // namespace Chimera
