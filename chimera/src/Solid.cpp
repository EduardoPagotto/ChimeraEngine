#include "Solid.h"
#include "Singleton.h"
#include "ChimeraUtils.h"
#include "Draw.h"
#include "NodeVisitor.h"

namespace Chimera {
    
Solid::Solid (Node *_parent, std::string _name ) : Coord (_parent, _name ) {

	setKind(EntityKind::PHYSICS);

    pRigidBody = nullptr;
    pShapeCollision = nullptr;
    pMotionState = nullptr;
    trimesh = nullptr;

    mass = 0.0f;
    friction = 0.0f;
    restitution = 0.0f;

	transform.setIdentity();

    pWorld = Infra::Singleton<PhysicsControl>::getRefSingleton();
}

Solid::Solid ( const Solid& _solid) : Coord (_solid) {

    mass = _solid.mass;
    friction = _solid.friction;
    restitution = _solid.restitution;

    pRigidBody = nullptr;
    pShapeCollision = _solid.pShapeCollision;
    pMotionState = nullptr;
    trimesh = nullptr;

	transform = _solid.transform;

    pWorld = Infra::Singleton<PhysicsControl>::getRefSingleton();
}

Solid::~Solid() {

    if ( pRigidBody ) {

        //pWorld->discretDynamicsWorld->removeRigidBody ( pRigidBody );
        pWorld->getWorld()->removeRigidBody ( pRigidBody );
        delete pRigidBody->getMotionState();
        delete pRigidBody;
    }

    if ( pShapeCollision ) {
        delete pShapeCollision;
    }

    Infra::Singleton<PhysicsControl>::releaseRefSingleton();
}

 void Solid::update ( DataMsg *_dataMsg ) {

	 if ((_dataMsg->getKindOp() == KindOp::DRAW) || (_dataMsg->getKindOp() == KindOp::DRAW_NO_TEX)) {

		 glPushMatrix();

		 Solid *pSource = (Solid *)_dataMsg->getParam();
		 ajusteMatrix( pSource );

		 Node::update(_dataMsg);

		 glPopMatrix();

	 }
	 else if (_dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE) {

		 _dataMsg->setDone(true);
		 //Node::update(_dataMsg);
	 }


 }

void Solid::init() {

	Draw *pDraw = (Draw*)Node::findChildByKind(EntityKind::DRAW, 0);

	if (isShapeDefine() == false)
		setShapeBox(pDraw->getSizeBox());

	initTransform(transform, this);

	Coord::init();

}

void Solid::accept(NodeVisitor * v) {
	v->visit(this);
}

void Solid::setPositionRotation(const btVector3 &_posicao, const btVector3 &_rotation) {

	btQuaternion l_qtn;
	transform.setIdentity();
	l_qtn.setEulerZYX(_rotation.getX(), _rotation.getY(), _rotation.getZ());
	transform.setRotation(l_qtn);
	transform.setOrigin(_posicao);
	//pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));
}

void Solid::initTransform( btTransform &_tTrans, void *pObj ) {

    pMotionState = new btDefaultMotionState ( _tTrans );

    btVector3 localInertia ( 0.0, 0.0, 0.0 );
    if ( mass != 0.0f ) {
        pShapeCollision->calculateLocalInertia ( mass, localInertia );
    }

    //pShapeCollision->setUserPointer((void*)this);

    btRigidBody::btRigidBodyConstructionInfo rBodyInfo ( mass, pMotionState, pShapeCollision, localInertia );
    pRigidBody = new btRigidBody ( rBodyInfo );

    pRigidBody->setActivationState ( DISABLE_DEACTIVATION );

    pRigidBody->setUserPointer ( ( void* ) pObj );

    pRigidBody->setFriction ( friction );
    pRigidBody->setRestitution ( restitution );

    pRigidBody->setContactProcessingThreshold ( BT_LARGE_FLOAT );

    //pWorld->discretDynamicsWorld->addRigidBody ( pRigidBody, 1, 1 );
    pWorld->getWorld()->addRigidBody ( pRigidBody, 1, 1 );

}

void Solid::setIndexVertexArray ( btTriangleIndexVertexArray *_indexVertexArray ) {

    trimesh = new btGImpactMeshShape ( _indexVertexArray );
    trimesh->setLocalScaling ( btVector3 ( 1.f, 1.f, 1.f ) );
    trimesh->updateBound();
    pShapeCollision = trimesh;
    // pShapeCollision->updateBound();
    //pShapeCollision = new pShapeCollision(trimesh);

}

void Solid::transformacao3D ( void ) {

    btTransform transLocal;
    btScalar matrix[16];

    pRigidBody->getMotionState()->getWorldTransform ( transLocal );
    transLocal.getOpenGLMatrix ( matrix );

    glMultMatrixf ( matrix );

}

void Solid::ajusteMatrix ( Solid *_pPhysic ) {

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

void Solid::setPosition ( const btVector3 &_pos ) {
    btTransform l_transform = pRigidBody->getCenterOfMassTransform();
    l_transform.setOrigin ( _pos );
    pRigidBody->setCenterOfMassTransform ( l_transform );
}

void Solid::setRotation ( const btVector3 &_rotation ) {
    btTransform transform = pRigidBody->getCenterOfMassTransform();

    transform.setRotation ( btQuaternion ( _rotation.getY(), _rotation.getX(), _rotation.getZ() ) );
    pRigidBody->setCenterOfMassTransform ( transform );
}

btVector3 Solid::getRotation() {

    btScalar rotZ, rotY, rotX;
    pRigidBody->getWorldTransform().getBasis().getEulerZYX ( rotZ, rotY, rotX );

    return btVector3 ( rotX, rotY, rotZ );
}

void Solid::applyTorc( const btVector3 &_torque ) {
    //pRigidBody->applyTorque(_torque);

    pRigidBody->applyTorque ( pRigidBody->getInvInertiaTensorWorld().inverse() * ( pRigidBody->getWorldTransform().getBasis() * _torque ) );

    //pRigidBody->getInvInertiaTensorWorld().inverse()*(pRigidBody->getWorldTransform().getBasis() * _torque);
    //RigidBody->getInvInertiaTensorWorld().inverse()*(pRigidBody->getWorldTransform().getBasis() * _torque);
}

void Solid::applyForce( const btVector3 &_prop ) {
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

void Solid::loadColladaShape ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, std::string &_meshName ) {

    if ( _nShape != nullptr ) {
        _nShape = _nShape->FirstChildElement();
        const char *l_tipoShape = _nShape->Value();

        if ( strcmp ( l_tipoShape, "sphere" ) == 0 ) {

            tinyxml2::XMLElement* l_nEsfera = _nShape->FirstChildElement();
            const char *l_raio = l_nEsfera->GetText();

            std::vector<float> l_arrayValores;
            loadArrayBtScalar ( l_raio, l_arrayValores );
            if ( l_arrayValores.size() == 1 ) {
                setShapeSphere ( l_arrayValores[0] );
            } else if ( l_arrayValores.size() == 3 ) {
                setShapeSphere ( l_arrayValores[0] );
            } else {

            }

        } else if ( strcmp ( l_tipoShape, "plane" ) == 0 ) {

            //setShapeBox(btVector3(2.0, 2.0, 2.0));

            tinyxml2::XMLElement* l_nBox = _nShape->FirstChildElement();
            const char *l_size = l_nBox->GetText();

            std::vector<float> l_arrayValores;
            loadArrayBtScalar ( l_size, l_arrayValores );

            if ( l_arrayValores.size() == 1 ) {
                setShapePlane ( btVector3 ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] ), l_arrayValores[0] );
            } else if ( l_arrayValores.size() == 4 ) {
                setShapePlane ( btVector3 ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] ), l_arrayValores[3] );
            } else {

            }

        } else if ( strcmp ( l_tipoShape, "box" ) == 0 ) {

            tinyxml2::XMLElement* l_nBox = _nShape->FirstChildElement();
            const char *l_size = l_nBox->GetText();

            std::vector<float> l_arrayValores;
            loadArrayBtScalar ( l_size, l_arrayValores );

            if ( l_arrayValores.size() == 1 ) {
                setShapeBox ( btVector3 ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] ) );
            } else if ( l_arrayValores.size() == 3 ) {
                setShapeBox ( btVector3 ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] ) );
            } else {

            }

        } else if ( strcmp ( l_tipoShape, "cylinder" ) == 0 ) {

            tinyxml2::XMLElement* l_nCyl = _nShape->FirstChildElement();
            const char *l_size = l_nCyl->GetText();

            std::vector<float> l_arrayValores;
            loadArrayBtScalar ( l_size, l_arrayValores );

            if ( l_arrayValores.size() == 1 ) {
                setShapeCilinder ( btVector3 ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] ) );
            } else if ( l_arrayValores.size() == 3 ) {
                setShapeCilinder ( btVector3 ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] ) );
            }
        } else if ( strcmp ( l_tipoShape, "mesh" ) == 0 ) { //FIXME ERRADO!!!!

            //setShapeBox(btVector3(1.0, 1.0, 1.0));

            //instance_geometry
            tinyxml2::XMLElement* l_nMesh = _nShape->FirstChildElement();
            if ( l_nMesh != nullptr ) {
                const char *l_mesh = l_nMesh->Attribute ( "url" );
                if ( l_mesh != nullptr ) {
                    _meshName = ( const char* ) &l_mesh[1];
                }
            }
        }
    }
}

//Transformacao quando Euley nao apagar
//btQuaternion l_qtn;
//m_trans.setIdentity();
//l_qtn.setEulerZYX ( _pTrans->getRotation().x(), _pTrans->getRotation().y(), _pTrans->getRotation().z() );
// m_trans.setRotation ( l_qtn );
// m_trans.setOrigin ( _pTrans->getPosition() );
//pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

void Solid::loadColladaPhysicsModel ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nRigidBody, std::string &_meshName ) {

    tinyxml2::XMLElement* l_nTecnicCommon = _nRigidBody->FirstChildElement ( "technique_common" );
    if ( l_nTecnicCommon != nullptr ) {

        //Massa
        tinyxml2::XMLElement* l_nMass = l_nTecnicCommon->FirstChildElement ( "mass" );
        if ( l_nMass != nullptr ) {
            const char* l_mass = l_nMass->GetText();
            setMass ( atof ( l_mass ) );
        }
    }

    //shape
    tinyxml2::XMLElement* l_nShape = l_nTecnicCommon->FirstChildElement ( "shape" );
    loadColladaShape ( _root, l_nShape, _meshName );

    //material
    tinyxml2::XMLElement* l_nInstaceMaterial = l_nTecnicCommon->FirstChildElement ( "instance_physics_material" );
    if ( l_nInstaceMaterial != nullptr ) {
        const char* l_url = l_nInstaceMaterial->Attribute ( "url" );

        tinyxml2::XMLElement* l_nMateriual = nullptr;
        Chimera::loadNodeLib ( _root, ( const char* ) &l_url[1], "library_physics_materials", "physics_material", &l_nMateriual );

        tinyxml2::XMLElement* l_nTec = l_nMateriual->FirstChildElement ( "technique_common" );
        if ( l_nTec != nullptr ) {

            tinyxml2::XMLElement* l_nFric = l_nTec->FirstChildElement ( "dynamic_friction" );
            if ( l_nFric != nullptr ) {
                const char *l_fric = l_nFric->GetText();
                if ( l_fric != nullptr ) {
                    setFriction ( atof ( l_fric ) );
                }
            }

            tinyxml2::XMLElement* l_nRes = l_nTec->FirstChildElement ( "restitution" );
            if ( l_nRes != nullptr ) {
                const char *l_res = l_nRes->GetText();
                if ( l_res != nullptr ) {
                    setRestitution ( atof ( l_res ) );
                }
            }
        }
    }
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;