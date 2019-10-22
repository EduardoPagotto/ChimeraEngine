#include "chimera/node/Solid.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/NodeVisitor.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

Solid::Solid(Node* _parent, std::string _name, PhysicsControl* _pWorld) : Coord(_parent, EntityKind::SOLID, _name) {

    pRigidBody = nullptr;
    pShapeCollision = nullptr;
    pMotionState = nullptr;
    trimesh = nullptr;

    mass = 0.0f;
    frictionDynamic = 15.0f;
    frictionStatic = 10.0f;
    restitution = 0.0f;

    transform.setIdentity();

    pWorld = _pWorld;
}

Solid::Solid(const Solid& _solid) : Coord(_solid) {

    mass = _solid.mass;

    frictionDynamic = _solid.frictionDynamic;
    frictionStatic = _solid.frictionStatic;
    restitution = _solid.restitution;

    pRigidBody = nullptr;
    pShapeCollision = _solid.pShapeCollision;
    pMotionState = nullptr;
    trimesh = nullptr;

    transform = _solid.transform;

    pWorld = _solid.pWorld;
}

Solid::~Solid() {

    if (pRigidBody) {

        // pWorld->discretDynamicsWorld->removeRigidBody ( pRigidBody );
        pWorld->getWorld()->removeRigidBody(pRigidBody);
        delete pRigidBody->getMotionState();
        delete pRigidBody;
    }

    if (pShapeCollision) {
        delete pShapeCollision;
    }
}

void Solid::init() {

    Mesh* pMesh = (Mesh*)Node::findChild(EntityKind::MESH, 0, false); // FIXME melhorar

    if (isShapeDefine() == false)
        setShapeBox(pMesh->getSizeBox());

    initTransform(transform, this);
}

void Solid::accept(NodeVisitor* v) { v->visit(this); }

// void Solid::setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation) {
//     btQuaternion l_qtn;
//     transform.setIdentity();
//     l_qtn.setEulerZYX(_rotation.x, _rotation.y, _rotation.z);
//     transform.setRotation(l_qtn);
//     transform.setOrigin(btVector3(_posicao.x, _posicao.y, _posicao.z));
//     // pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
//     // l_posicao));
// }

void Solid::initTransform(const btTransform& _tTrans, void* pObj) {

    // btTransform trans;
    // trans.getOpenGLMatrix( glm::value_ptr( _tTrans )  );

    pMotionState = new btDefaultMotionState(_tTrans);

    btVector3 localInertia(0.0, 0.0, 0.0);
    if (mass != 0.0f) {
        pShapeCollision->calculateLocalInertia(mass, localInertia);
    }

    // pShapeCollision->setUserPointer((void*)this);

    btRigidBody::btRigidBodyConstructionInfo rBodyInfo(mass, pMotionState, pShapeCollision, localInertia);
    pRigidBody = new btRigidBody(rBodyInfo);

    pRigidBody->setActivationState(DISABLE_DEACTIVATION);

    pRigidBody->setUserPointer((void*)pObj);

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

glm::mat4 Solid::getModelMatrix(Coord* _pCoord) { // ajuste matricial

    btTransform transLocal;
    btScalar matrix[16];

    // Pega posicao do corpo atual e ajusta sua matrix (posicao e rotacao)
    pRigidBody->getMotionState()->getWorldTransform(transLocal);
    transLocal.getOpenGLMatrix(&matrix[0]);

    // pega posicao do objeto horigem de desenho (camera travada)
    glm::vec3 l_vec = _pCoord->getPosition();

    // desloca desenha para o pbjeto horigem
    matrix[12] -= l_vec.x;
    matrix[13] -= l_vec.y;
    matrix[14] -= l_vec.z;

    glm::mat4 tempMat = glm::make_mat4(matrix);

    // glMultMatrixf (glm::value_ptr(tempMat));
    // glMultMatrixf(matrix);

    return tempMat;
}

glm::vec3 Solid::getPosition() {
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

glm::vec3 Solid::getRotation() {

    btScalar rotZ, rotY, rotX;
    pRigidBody->getWorldTransform().getBasis().getEulerZYX(rotZ, rotY, rotX);

    return glm::vec3(rotX, rotY, rotZ);
}

glm::mat4 Solid::getMatrix() {

    btTransform transLocal;
    btScalar matrix[16];

    // Pega posicao do corpo atual e ajusta sua matrix (posicao e rotacao)
    pRigidBody->getMotionState()->getWorldTransform(transLocal);
    transLocal.getOpenGLMatrix(&matrix[0]);

    // glMultMatrixf (glm::value_ptr(tempMat));
    // glMultMatrixf(matrix);

    return glm::make_mat4(matrix);
    ;
}

void Solid::setMatrix(const glm::mat4& _trans) { transform.setFromOpenGLMatrix((btScalar*)glm::value_ptr(_trans)); }

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
