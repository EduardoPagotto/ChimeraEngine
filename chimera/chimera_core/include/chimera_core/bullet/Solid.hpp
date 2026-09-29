#pragma once
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include "PhysicsControl.hpp"
#include "chimera_base/aux/Transform.hpp"
#include <BulletCollision/CollisionShapes/btMaterial.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>

namespace ce {

    class Solid : public ITrans {
      public:
        Solid(PhysicsControl* _pWorld, const glm::mat4& _trans, uint32_t entity);
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
        void init(const glm::vec3& _size); // usado no scene no final da inicializacao
        // prop shape
        inline void setShapeBox(const glm::vec3& _size) {
            p_shape_collision_ = new btBoxShape(btVector3(_size.x, _size.y, _size.z));
        }
        inline void setShapeCilinder(const glm::vec3& _val) {
            p_shape_collision_ = new btCylinderShape(btVector3(_val.x, _val.y, _val.z));
        }
        inline void setShapePlane(const glm::vec3& _val, const float& _constant) {
            p_shape_collision_ = new btStaticPlaneShape(btVector3(_val.x, _val.y, _val.z), _constant);
        }
        inline void setShapeSphere(float _raio) { p_shape_collision_ = new btSphereShape((btScalar)_raio); }
        void setIndexVertexArray(btTriangleIndexVertexArray* _indexVertexArray);
        bool isShapeDefine() { return (p_shape_collision_ != nullptr ? true : false); }

        inline void setMass(const float& _mass) { mass_ = _mass; }

        void applyForce(const glm::vec3& _prop);
        void applyTorc(const glm::vec3& _torque);
        inline void setFrictionDynamic(const float& _friction) { friction_dynamic_ = _friction; }
        inline void setFrictionStatic(const float& _friction) { friction_static_ = _friction; }
        inline void setRestitution(const float& _restitution) { restitution_ = _restitution; }

      private:
        btScalar mass_;
        btScalar friction_dynamic_;
        btScalar friction_static_;
        btScalar restitution_;

        btRigidBody* p_rigid_body_;
        btCollisionShape* p_shape_collision_;
        btGImpactMeshShape* trimesh_;
        btDefaultMotionState* p_motion_state_;
        PhysicsControl* p_world_;
        // btTriangleIndexVertexArray *indexVertexArray;
        // btTriangleIndexVertexArray *m_pIndexVertexArrays;
        uint32_t entity_;
    };
} // namespace ce
