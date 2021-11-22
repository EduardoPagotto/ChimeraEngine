#include "chimera/render/bullet/Solid.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

Solid::Solid(PhysicsControl* _pWorld, const glm::mat4& _trans, uint32_t entity)
    : pWorld(_pWorld), pRigidBody(nullptr), pShapeCollision(nullptr), trimesh(nullptr), mass(0.0f), frictionDynamic(15.0f),
      frictionStatic(10.0f), restitution(0.0f), entity(entity) {

    this->setMatrix(_trans); // pMotionState carregado aqui!
}

Solid::~Solid() {

    if (pRigidBody) {
        // FIXME: esta falhando aqui!!!
        // pWorld->discretDynamicsWorld->removeRigidBody ( pRigidBody );
        // pWorld->getWorld()->removeRigidBody(pRigidBody);
        // delete pRigidBody->getMotionState();
        // delete pRigidBody;
    }

    if (pShapeCollision) {
        delete pShapeCollision;
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

void Solid::init(const glm::vec3& _size) {

    if (isShapeDefine() == false)
        setShapeBox(_size);

    btVector3 localInertia(0.0, 0.0, 0.0);
    if (mass != 0.0f) {
        pShapeCollision->calculateLocalInertia(mass, localInertia);
    }

    pShapeCollision->setUserPointer((void*)&entity);

    btRigidBody::btRigidBodyConstructionInfo rBodyInfo(mass, pMotionState, pShapeCollision, localInertia);
    pRigidBody = new btRigidBody(rBodyInfo);

    pRigidBody->setActivationState(DISABLE_DEACTIVATION);

    pRigidBody->setUserPointer((void*)&entity);

    // TODO: implementar o atrito estatico
    pRigidBody->setFriction(frictionDynamic);
    pRigidBody->setRestitution(restitution);

    pRigidBody->setContactProcessingThreshold(BT_LARGE_FLOAT);

    // pWorld->discretDynamicsWorld->addRigidBody ( pRigidBody, 1, 1 );
    pWorld->getWorld()->addRigidBody(pRigidBody, 1, 1);
}

void Solid::setIndexVertexArray(btTriangleIndexVertexArray* _indexVertexArray) {

    trimesh = new btGImpactMeshShape(_indexVertexArray);
    trimesh->setLocalScaling(btVector3(1.f, 1.f, 1.f));
    trimesh->updateBound();
    pShapeCollision = trimesh;
    // pShapeCollision->updateBound();
    // pShapeCollision = new pShapeCollision(trimesh);
}

const glm::mat4 Solid::translateSrc(const glm::vec3& _pos) const { // translate model matrix
    btTransform transLocal;
    btScalar matrix[16];

    // Pega posicao do corpo atual e ajusta sua matrix (posicao e rotacao)
    pRigidBody->getMotionState()->getWorldTransform(transLocal);
    transLocal.getOpenGLMatrix(&matrix[0]);

    // desloca desenha para o pbjeto horigem
    matrix[12] -= _pos.x;
    matrix[13] -= _pos.y;
    matrix[14] -= _pos.z;

    return glm::make_mat4(matrix);
}

const glm::vec3 Solid::getPosition() const {
    btVector3 pos = pRigidBody->getWorldTransform().getOrigin();
    return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}

void Solid::setPosition(const glm::vec3& _pos) {

    btTransform l_transform = pRigidBody->getCenterOfMassTransform();
    l_transform.setOrigin(btVector3(_pos.x, _pos.y, _pos.z));
    pRigidBody->setCenterOfMassTransform(l_transform);
}

void Solid::setRotation(const glm::vec3& _rotation) {

    btTransform transform = pRigidBody->getCenterOfMassTransform();

    transform.setRotation(btQuaternion(_rotation.y, _rotation.x, _rotation.z));
    pRigidBody->setCenterOfMassTransform(transform);
}

// glm::vec3 Solid::getRotation() {
//     btScalar rotZ, rotY, rotX;
//     pRigidBody->getWorldTransform().getBasis().getEulerZYX(rotZ, rotY, rotX);
//     return glm::vec3(rotX, rotY, rotZ);
// }

const glm::mat4 Solid::getMatrix() const {

    btTransform transLocal;
    btScalar matrix[16];
    // Pega posicao do corpo atual e ajusta sua matrix (posicao e rotacao)
    pRigidBody->getMotionState()->getWorldTransform(transLocal);
    transLocal.getOpenGLMatrix(&matrix[0]);

    return glm::make_mat4(matrix);
}

void Solid::setMatrix(const glm::mat4& _trans) {
    btTransform transform;
    transform.setFromOpenGLMatrix((btScalar*)glm::value_ptr(_trans));
    pMotionState = new btDefaultMotionState(transform);
}

void Solid::applyTorc(const glm::vec3& _torque) {
    // pRigidBody->applyTorque(_torque);

    pRigidBody->applyTorque(pRigidBody->getInvInertiaTensorWorld().inverse() *
                            (pRigidBody->getWorldTransform().getBasis() * btVector3(_torque.x, _torque.y, _torque.z)));

    // pRigidBody->getInvInertiaTensorWorld().inverse()*(pRigidBody->getWorldTransform().getBasis()
    // * _torque);
    // RigidBody->getInvInertiaTensorWorld().inverse()*(pRigidBody->getWorldTransform().getBasis()
    // * _torque);
}

void Solid::applyForce(const glm::vec3& _prop) {
    // Jeito um
    // btTransform boxTrans;
    // pRigidBody->getMotionState()->getWorldTransform(boxTrans);
    // btVector3 correctedForce = (boxTrans *_prop) - boxTrans.getOrigin();
    // pRigidBody->applyCentralForce(correctedForce);

    // Jeito 2
    btMatrix3x3& boxRot = pRigidBody->getWorldTransform().getBasis();

    btVector3 prop(_prop.x, _prop.y, _prop.z);

    btVector3 correctedForce = boxRot * prop;
    pRigidBody->applyCentralForce(correctedForce);
}

// Transformacao quando Euley nao apagar
// btQuaternion l_qtn;
// m_trans.setIdentity();
// l_qtn.setEulerZYX ( _pTrans->getRotation().x(), _pTrans->getRotation().y(),
// _pTrans->getRotation().z() );
// m_trans.setRotation ( l_qtn );
// m_trans.setOrigin ( _pTrans->getPosition() );
// pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

} // namespace Chimera
