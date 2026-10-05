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
        inline void set_shape_box(const glm::vec3& size) {
            p_shape_collision_ = new btBoxShape(btVector3(size.x, size.y, size.z));
        }
        inline void set_shape_cilinder(const glm::vec3& val) {
            p_shape_collision_ = new btCylinderShape(btVector3(val.x, val.y, val.z));
        }
        inline void set_shape_plane(const glm::vec3& val, const float& constant) {
            p_shape_collision_ = new btStaticPlaneShape(btVector3(val.x, val.y, val.z), constant);
        }
        inline void set_shape_sphere(float raio) { p_shape_collision_ = new btSphereShape((btScalar)raio); }
        void set_index_vertex_array(btTriangleIndexVertexArray* index_vertex_array);
        bool is_shape_define() { return (p_shape_collision_ != nullptr ? true : false); }

        inline void set_mass(const float& mass) { mass_ = mass; }

        void apply_force(const glm::vec3& prop);
        void apply_torc(const glm::vec3& torque);
        inline void set_friction_dynamic(const float& friction) { friction_dynamic_ = friction; }
        inline void set_friction_static(const float& friction) { friction_static_ = friction; }
        inline void set_restitution(const float& restitution) { restitution_ = restitution; }

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
