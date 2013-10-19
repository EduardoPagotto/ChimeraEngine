#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletCollision/CollisionShapes/btMaterial.h>

#include "PhysicWorld.h"
#include "Singleton.h"


namespace Chimera {

class Physics {
public:
    friend class Loader;

    Physics();
    ~Physics();

    inline void setMass ( float _mass ) {
        if ( _mass > 0 )
            m_estatica = false;

        m_mass = _mass;
    }

    void setShapeBox ( const btVector3 &_size ) {
        m_pCollisionShape = new btBoxShape ( _size );
    }

    void init ( btTransform *_pBtTrans );

    //usada na trans cam do mundo
    void transformacao3D ( void );

    //usada na trans da cam objeto
    void ajusteMatrix ( Physics *_pPhysic );

    void propulcao ( const btVector3 &_prop );
    void torque ( const btVector3 &_torque );

    //FIXME depois 
public:

    btRigidBody* m_pRigidBody;

    bool m_estatica;

    btScalar m_mass;
    btVector3 m_inercial;
    //btTransform m_trans;
    btTransform *pFisicTransform;
    
    btScalar m_matrix[16];

    //btVector3 *m_pPosicao;
    //btVector3 *m_pRotacao;
    //Transform *m_pTrans;

    btCollisionShape* m_pCollisionShape;

    btTriangleIndexVertexArray *m_pIndexVertexArrays;

    btDefaultMotionState *m_pMotionState;

    btRigidBody::btRigidBodyConstructionInfo *m_pRigidBodyCI;

    PhysicWorld *m_physicWorld;

    btMaterial *m_pPhysicMaterial;
};

}
#endif //Physics_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
