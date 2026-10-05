#include "chimera_core/bullet/Solid.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ce {

    Solid::Solid(PhysicsControl* p_world, const glm::mat4& trans, uint32_t entity)
        : mass_(0.0f), friction_dynamic_(15.0f), friction_static_(10.0f), restitution_(0.0f), p_rigid_body_(nullptr),
          p_shape_collision_(nullptr), trimesh_(nullptr), p_world_(p_world), entity_(entity) {

        this->set_matrix(trans); // pMotionState carregado aqui!
    }

    Solid::~Solid() {

        if (p_rigid_body_) {
            // FIXME: esta falhando aqui!!!
            // pWorld->discretDynamicsWorld->removeRigidBody ( pRigidBody );
            // pWorld->getWorld()->removeRigidBody(pRigidBody);
            // delete pRigidBody->getMotionState();
            // delete pRigidBody;
        }

        if (p_shape_collision_) {
            delete p_shape_collision_;
        }
    }

    // TODO: verificar depois
    // void Solid::setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {
    //     btQuaternion l_qtn;
    //     transform.setIdentity();
    //     l_qtn.setEulerZYX(_rotation.x, _rotation.y, _rotation.z);
    //     transform.setRotation(l_qtn);
    //     transform.setOrigin(btVector3(_posicao.x, _posicao.y, _posicao.z));
    //     // pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
    //     // l_posicao));
    // }

    void Solid::init(const glm::vec3& size) {

        if (is_shape_define() == false)
            set_shape_box(size);

        btVector3 local_inertia(0.0, 0.0, 0.0);
        if (mass_ != 0.0f) {
            p_shape_collision_->calculateLocalInertia(mass_, local_inertia);
        }

        p_shape_collision_->setUserPointer((void*)&entity_);

        btRigidBody::btRigidBodyConstructionInfo r_body_info(mass_, p_motion_state_, p_shape_collision_, local_inertia);
        p_rigid_body_ = new btRigidBody(r_body_info);

        p_rigid_body_->setActivationState(DISABLE_DEACTIVATION);

        p_rigid_body_->setUserPointer((void*)&entity_);

        // TODO: implementar o atrito estatico
        p_rigid_body_->setFriction(friction_dynamic_);
        p_rigid_body_->setRestitution(restitution_);

        p_rigid_body_->setContactProcessingThreshold(BT_LARGE_FLOAT);

        // pWorld->discretDynamicsWorld->addRigidBody ( pRigidBody, 1, 1 );
        p_world_->get_world()->addRigidBody(p_rigid_body_, 1, 1);
    }

    void Solid::set_index_vertex_array(btTriangleIndexVertexArray* index_vertex_array) {

        trimesh_ = new btGImpactMeshShape(index_vertex_array);
        trimesh_->setLocalScaling(btVector3(1.f, 1.f, 1.f));
        trimesh_->updateBound();
        p_shape_collision_ = trimesh_;
        // pShapeCollision->updateBound();
        // pShapeCollision = new pShapeCollision(trimesh);
    }

    const glm::mat4 Solid::translate_src(const glm::vec3& pos) const { // translate model matrix
        btTransform trans_local;
        btScalar matrix[16];

        // Pega posicao do corpo atual e ajusta sua matrix (posicao e rotacao)
        p_rigid_body_->getMotionState()->getWorldTransform(trans_local);
        trans_local.getOpenGLMatrix(&matrix[0]);

        // desloca desenha para o pbjeto horigem
        matrix[12] -= pos.x;
        matrix[13] -= pos.y;
        matrix[14] -= pos.z;

        return glm::make_mat4(matrix);
    }

    const glm::vec3 Solid::get_position() const {
        btVector3 pos = p_rigid_body_->getWorldTransform().getOrigin();
        return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
    }

    void Solid::set_position(const glm::vec3& pos) {

        btTransform l_transform = p_rigid_body_->getCenterOfMassTransform();
        l_transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
        p_rigid_body_->setCenterOfMassTransform(l_transform);
    }

    void Solid::set_rotation(const glm::vec3& rotation) {

        btTransform transform = p_rigid_body_->getCenterOfMassTransform();

        transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
        p_rigid_body_->setCenterOfMassTransform(transform);
    }

    // glm::vec3 Solid::getRotation() {
    //     btScalar rotZ, rotY, rotX;
    //     pRigidBody->getWorldTransform().getBasis().getEulerZYX(rotZ, rotY, rotX);
    //     return glm::vec3(rotX, rotY, rotZ);
    // }

    const glm::mat4 Solid::get_matrix() const {

        btTransform trans_local;
        btScalar matrix[16];
        // Pega posicao do corpo atual e ajusta sua matrix (posicao e rotacao)
        p_rigid_body_->getMotionState()->getWorldTransform(trans_local);
        trans_local.getOpenGLMatrix(&matrix[0]);

        return glm::make_mat4(matrix);
    }

    void Solid::set_matrix(const glm::mat4& trans) {
        btTransform transform;
        transform.setFromOpenGLMatrix((btScalar*)glm::value_ptr(trans));
        p_motion_state_ = new btDefaultMotionState(transform);
    }

    void Solid::apply_torc(const glm::vec3& torque) {
        // pRigidBody->applyTorque(_torque);

        p_rigid_body_->applyTorque(
            p_rigid_body_->getInvInertiaTensorWorld().inverse() *
            (p_rigid_body_->getWorldTransform().getBasis() * btVector3(torque.x, torque.y, torque.z)));

        // pRigidBody->getInvInertiaTensorWorld().inverse()*(pRigidBody->getWorldTransform().getBasis()
        // * _torque);
        // RigidBody->getInvInertiaTensorWorld().inverse()*(pRigidBody->getWorldTransform().getBasis()
        // * _torque);
    }

    void Solid::apply_force(const glm::vec3& prop) {
        // Jeito um
        // btTransform boxTrans;
        // pRigidBody->getMotionState()->getWorldTransform(boxTrans);
        // btVector3 correctedForce = (boxTrans *_prop) - boxTrans.getOrigin();
        // pRigidBody->applyCentralForce(correctedForce);

        // Jeito 2
        btMatrix3x3& box_rot = p_rigid_body_->getWorldTransform().getBasis();

        btVector3 l_prop(prop.x, prop.y, prop.z);

        btVector3 corrected_force = box_rot * (l_prop);
        p_rigid_body_->applyCentralForce(corrected_force);
    }

    // Transformacao quando Euley nao apagar
    // btQuaternion l_qtn;
    // m_trans.setIdentity();
    // l_qtn.setEulerZYX ( _pTrans->getRotation().x(), _pTrans->getRotation().y(),
    // _pTrans->getRotation().z() );
    // m_trans.setRotation ( l_qtn );
    // m_trans.setOrigin ( _pTrans->getPosition() );
    // pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

} // namespace ce
