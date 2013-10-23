#include "Physics.h"

namespace Chimera {

Physics::Physics() {
    
    m_pRigidBody = nullptr;
    m_pCollisionShape = nullptr;
    m_pPhysicMaterial = nullptr;
    m_mass = 0.0f;
    m_inercial.setZero();
    m_estatica = true;
   
    m_physicWorld = Singleton<PhysicWorld>::getRefSingleton();
    
}

Physics::~Physics() {
    
    if ( m_pRigidBody ) {
        m_physicWorld->m_pDynamicsWorld->removeRigidBody ( m_pRigidBody );
        delete m_pRigidBody->getMotionState();
        delete m_pRigidBody;
    }

    if ( m_pCollisionShape )
        delete m_pCollisionShape;

    Singleton<PhysicWorld>::releaseRefSingleton();
}

void Physics::init ( btTransform *_pBtTrans, unsigned _serial ) {
    
    //Transformacao quando Euley nao apagar
    //btQuaternion l_qtn;
    //m_trans.setIdentity();
    //l_qtn.setEulerZYX ( _pTrans->getRotation().x(), _pTrans->getRotation().y(), _pTrans->getRotation().z() );
   // m_trans.setRotation ( l_qtn );
   // m_trans.setOrigin ( _pTrans->getPosition() );
    pFisicTransform = _pBtTrans;
   
    m_pMotionState = new btDefaultMotionState ( *pFisicTransform );
    //m_pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

    btVector3 localInertia ( 0.0, 0.0, 0.0 );
    if ( m_estatica==false )
        m_pCollisionShape->calculateLocalInertia ( m_mass,localInertia );
    
    btRigidBody::btRigidBodyConstructionInfo rBodyInfo( m_mass, m_pMotionState, m_pCollisionShape ,localInertia );
    m_pRigidBody = new btRigidBody ( rBodyInfo );

   // m_pRigidBody->setContactProcessingThreshold(BT_LARGE_FLOAT);
    m_pRigidBody->setActivationState ( DISABLE_DEACTIVATION );

    m_pRigidBody->setUserPointer((void*)&_serial);
    
    if ( m_pPhysicMaterial ) {
        m_pRigidBody->setFriction ( m_pPhysicMaterial->m_friction );
        m_pRigidBody->setRestitution ( m_pPhysicMaterial->m_restitution );
    }

    m_physicWorld->m_pDynamicsWorld->addRigidBody ( m_pRigidBody, 1, 1 );

}

void Physics::transformacao3D ( void ) {
    m_pRigidBody->getMotionState()->getWorldTransform ( *pFisicTransform );
    pFisicTransform->getOpenGLMatrix ( m_matrix );
    glMultMatrixf ( m_matrix );
}

void Physics::ajusteMatrix ( Physics *_pPhysic ) {
    
    m_pRigidBody->getMotionState()->getWorldTransform ( *pFisicTransform );
    btVector3 l_vec = _pPhysic->m_pRigidBody->getWorldTransform().getOrigin();
    pFisicTransform->getOpenGLMatrix ( m_matrix );

    m_matrix[12] -= l_vec.getX();
    m_matrix[13] -= l_vec.getY();
    m_matrix[14] -= l_vec.getZ();

    glMultMatrixf ( m_matrix );
}

void Physics::torque ( const btVector3 &_torque ) {
    m_pRigidBody->applyTorque ( _torque );
}

void Physics::propulcao ( const btVector3 &_prop ) {
    //Jeito um
    //btTransform boxTrans;
    //m_pRigidBody->getMotionState()->getWorldTransform(boxTrans);
    //btVector3 correctedForce = (boxTrans *_prop) - boxTrans.getOrigin();
    //m_pRigidBody->applyCentralForce(correctedForce);

    //Jeito 2
    btMatrix3x3& boxRot = m_pRigidBody->getWorldTransform().getBasis();
    btVector3 correctedForce = boxRot *_prop;
    m_pRigidBody->applyCentralForce ( correctedForce );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
