#include "Physics.h"

namespace Chimera {

Physics::Physics(std::string _id, std::string _name) : Node(EntityKind::PHYSICS,_id,_name) {
    
    pRigidBody = nullptr;
    pShapeCollision = nullptr;
    pMotionState = nullptr;
    
    mass = 0.0f;
    friction = 0.0f;
    restitution = 0.0f;
    
    pWorld = Singleton<PhysicsControl>::getRefSingleton();
    
}

Physics::Physics(const Physics& _physics) : Node(_physics) {
    
    mass = _physics.mass;
    friction = _physics.friction;
    restitution = _physics.restitution;

    pRigidBody = nullptr;
    pShapeCollision = nullptr;
    pMotionState = nullptr;    
    
    pWorld = Singleton<PhysicsControl>::getRefSingleton();
}

Physics::~Physics() {
    
    if ( pRigidBody ) {
        pWorld->discretDynamicsWorld->removeRigidBody ( pRigidBody );
        delete pRigidBody->getMotionState();
        delete pRigidBody;
    }

    if ( pShapeCollision )
        delete pShapeCollision;

    Singleton<PhysicsControl>::releaseRefSingleton();
}

void Physics::clone(Node **ppNode ) {
    *ppNode = new Physics( *this ); 
    Node::clone( ppNode );  
}

void Physics::update ( DataMsg *_dataMsg ) {
    Node::update(_dataMsg);
}

void Physics::init (btTransform &_tTrans, void *pObj ) {
        
    pMotionState = new btDefaultMotionState ( _tTrans );

    btVector3 localInertia ( 0.0, 0.0, 0.0 );
    if ( mass != 0.0f )
        pShapeCollision->calculateLocalInertia ( mass,localInertia );
    
    //pShapeCollision->setUserPointer((void*)this);
    
    btRigidBody::btRigidBodyConstructionInfo rBodyInfo( mass, pMotionState, pShapeCollision ,localInertia );
    pRigidBody = new btRigidBody ( rBodyInfo );

    pRigidBody->setActivationState ( DISABLE_DEACTIVATION );
    
    pRigidBody->setUserPointer((void*)pObj);

    pRigidBody->setFriction ( friction );
    pRigidBody->setRestitution ( restitution );
    //pRigidBody->setContactProcessingThreshold(BT_LARGE_FLOAT);  

    pWorld->discretDynamicsWorld->addRigidBody ( pRigidBody, 1, 1 );

}

void Physics::setIndexVertexArray(btTriangleIndexVertexArray *_indexVertexArray) {
    
    trimesh = new btGImpactMeshShape(_indexVertexArray);
    trimesh->updateBound();
    pShapeCollision = trimesh;
    // pShapeCollision->updateBound();
    //pShapeCollision = new pShapeCollision(trimesh);
    
}

void Physics::transformacao3D ( void ) {
    
    btTransform transLocal;
    btScalar matrix[16];

    pRigidBody->getMotionState()->getWorldTransform ( transLocal );
    transLocal.getOpenGLMatrix ( matrix );
    
    glMultMatrixf ( matrix );
    
}

void Physics::ajusteMatrix ( Physics *_pPhysic ) {
    
    btTransform transLocal;
    btScalar matrix[16];
    
    //Pega posicao do corpo atual e ajusta sua matrix (posicao e rotacao)
    pRigidBody->getMotionState()->getWorldTransform ( transLocal );
    transLocal.getOpenGLMatrix ( matrix );
    
    //pega posicao do objeto horigem de desenho (camera travada)
    btVector3 l_vec = _pPhysic->pRigidBody->getWorldTransform().getOrigin();
    
    //desloca desenha para o pbjeto horigem
    matrix[12] -= l_vec.getX();
    matrix[13] -= l_vec.getY();
    matrix[14] -= l_vec.getZ();

    glMultMatrixf ( matrix );
    
}

void Physics::setPosition(const btVector3 &_pos) {
    btTransform l_transform = pRigidBody->getCenterOfMassTransform();
    l_transform.setOrigin(_pos);
    pRigidBody->setCenterOfMassTransform(l_transform);
}

void Physics::setRotation(const btVector3 &_rotation){
    btTransform transform = pRigidBody->getCenterOfMassTransform();
    
    transform.setRotation(btQuaternion(_rotation.getY(), _rotation.getX(), _rotation.getZ()));
    pRigidBody->setCenterOfMassTransform(transform);
}

btVector3 Physics::getRotation(){

    btScalar rotZ,rotY,rotX;    
    pRigidBody->getWorldTransform().getBasis().getEulerZYX(rotZ, rotY, rotX);
    
    return btVector3(rotX,rotY,rotZ);
}

void Physics::torque ( const btVector3 &_torque ) {
    pRigidBody->applyTorque ( _torque );
}

void Physics::propulcao ( const btVector3 &_prop ) {
    //Jeito um
    //btTransform boxTrans;
    //pRigidBody->getMotionState()->getWorldTransform(boxTrans);
    //btVector3 correctedForce = (boxTrans *_prop) - boxTrans.getOrigin();
    //pRigidBody->applyCentralForce(correctedForce);

    //Jeito 2
    btMatrix3x3& boxRot = pRigidBody->getWorldTransform().getBasis();
    btVector3 correctedForce = boxRot *_prop;
    pRigidBody->applyCentralForce ( correctedForce );
    
}

    //Transformacao quando Euley nao apagar
    //btQuaternion l_qtn;
    //m_trans.setIdentity();
    //l_qtn.setEulerZYX ( _pTrans->getRotation().x(), _pTrans->getRotation().y(), _pTrans->getRotation().z() );
   // m_trans.setRotation ( l_qtn );
   // m_trans.setOrigin ( _pTrans->getPosition() );
    //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
