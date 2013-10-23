#include "Physics.h"

namespace Chimera {

Physics::Physics() {
    
    pRigidBody = nullptr;
    pShapeCollision = nullptr;
    m_pPhysicMaterial = nullptr;
    mass = 0.0f;
   
    m_physicWorld = Singleton<PhysicWorld>::getRefSingleton();
    
}

Physics::~Physics() {
    
    if ( pRigidBody ) {
        m_physicWorld->m_pDynamicsWorld->removeRigidBody ( pRigidBody );
        delete pRigidBody->getMotionState();
        delete pRigidBody;
    }

    if ( pShapeCollision )
        delete pShapeCollision;

    Singleton<PhysicWorld>::releaseRefSingleton();
}

void Physics::init (btTransform &_tTrans, unsigned _serial ) {
    
    //Transformacao quando Euley nao apagar
    //btQuaternion l_qtn;
    //m_trans.setIdentity();
    //l_qtn.setEulerZYX ( _pTrans->getRotation().x(), _pTrans->getRotation().y(), _pTrans->getRotation().z() );
   // m_trans.setRotation ( l_qtn );
   // m_trans.setOrigin ( _pTrans->getPosition() );
    
    //pFisicTransform = &_tTrans;
   
    m_pMotionState = new btDefaultMotionState ( _tTrans );
    //m_pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

    btVector3 localInertia ( 0.0, 0.0, 0.0 );
    if ( mass != 0.0f )
        pShapeCollision->calculateLocalInertia ( mass,localInertia );
    
    btRigidBody::btRigidBodyConstructionInfo rBodyInfo( mass, m_pMotionState, pShapeCollision ,localInertia );
    pRigidBody = new btRigidBody ( rBodyInfo );

   // pRigidBody->setContactProcessingThreshold(BT_LARGE_FLOAT);
    pRigidBody->setActivationState ( DISABLE_DEACTIVATION );

    pRigidBody->setUserPointer((void*)&_serial);
    
    if ( m_pPhysicMaterial ) {
        pRigidBody->setFriction ( m_pPhysicMaterial->m_friction );
        pRigidBody->setRestitution ( m_pPhysicMaterial->m_restitution );
    }

    m_physicWorld->m_pDynamicsWorld->addRigidBody ( pRigidBody, 1, 1 );

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

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
