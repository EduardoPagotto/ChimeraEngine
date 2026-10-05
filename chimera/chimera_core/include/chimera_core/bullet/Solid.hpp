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
        Solid(PhysicsControl* p_world, const glm::mat4& trans, uint32_t entity);
        virtual ~Solid();

        // Inherited
        virtual const glm::vec3 get_position() const override;
        virtual const glm::mat4 get_matrix() const override;
        virtual const glm::mat4 translate_src(const glm::vec3& pos) const override;
        // virtual const glm::vec3 getRotation() const override;
        virtual void set_position(const glm::vec3& pos) override;
        virtual void set_rotation(const glm::vec3& rotation) override;
        virtual void set_matrix(const glm::mat4& trans) override;

        // prop init FIXME: melhorar!!! ainda confuso
        void init(const glm::vec3& size); // usado no scene no final da inicializacao
        // prop shape
        inline void setShapeBox(const glm::vec3& size) {
            p_shape_collision_ = new btBoxShape(btVector3(size.x, size.y, size.z));
        }
        inline void setShapeCilinder(const glm::vec3& val) {
            p_shape_collision_ = new btCylinderShape(btVector3(val.x, val.y, val.z));
        }
        inline void setShapePlane(const glm::vec3& val, const float& constant) {
            p_shape_collision_ = new btStaticPlaneShape(btVector3(val.x, val.y, val.z), constant);
        }
        inline void setShapeSphere(float raio) { p_shape_collision_ = new btSphereShape((btScalar)raio); }
        void setIndexVertexArray(btTriangleIndexVertexArray* index_vertex_array);
        bool isShapeDefine() { return (p_shape_collision_ != nullptr ? true : false); }

        inline void setMass(const float& mass) { mass_ = mass; }

        void applyForce(const glm::vec3& prop);
        void applyTorc(const glm::vec3& torque);
        inline void setFrictionDynamic(const float& friction) { friction_dynamic_ = friction; }
        inline void setFrictionStatic(const float& friction) { friction_static_ = friction; }
        inline void setRestitution(const float& restitution) { restitution_ = restitution; }

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
