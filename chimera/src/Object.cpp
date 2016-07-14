#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>

#include "Object.h"
#include "NodeVisitor.h"

namespace Chimera {
    
Object::Object (Node* _parent, std::string _name ) : Node (_parent, EntityKind::OBJECT, _name ) {

    pPhysic = nullptr;
    pDraw = nullptr;
	pMaterial = nullptr;
    pTexture = nullptr;
    
    transform.setIdentity();
}

Object::~Object() {
}

void Object::setPositionRotation ( const btVector3 &_posicao, const btVector3 &_rotation ) {

    //Transformacao quando Euley nao apagar
    btQuaternion l_qtn;
    transform.setIdentity();
    l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
    transform.setRotation ( l_qtn );
    transform.setOrigin ( _posicao );
    //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

}

void Object::init() {
    
    if (pTexture != nullptr)
        pTexture->init();
        
	if (pMaterial == nullptr)
		pMaterial = new Material( "DefaultMat");

	pMaterial->init();

	if (pPhysic == nullptr) {

		//Cria corpo caso nao exista
		pPhysic = new Physics( "");
		pPhysic->setMass(0.5f);
		pPhysic->setFriction(0.0f);
		pPhysic->setRestitution(0.0f);
	}

	if (pDraw != nullptr) {
		pPhysic->setShapeBox(pDraw->getSizeBox());
	}

	if (pPhysic->isShapeDefine() == false) {
		pPhysic->setShapeBox(pDraw->getSizeBox());
	}

	pPhysic->initTransform(transform, this);

	Node::init();
}

void Object::accept(NodeVisitor * v) {
	v->visit(this);
}

void Object::execute ( bool _texture, Object *pObj ) {

    if ( pPhysic != nullptr ) {
        pPhysic->ajusteMatrix ( pObj->pPhysic );
    }

    if (( _texture == true ) && (pTexture != nullptr))
        pTexture->begin();
    
    pMaterial->apply();
   
	pDraw->renderExecute(_texture);
    
    if (( _texture == true ) && (pTexture != nullptr))
        pTexture->end();

}

void Object::update ( DataMsg *_dataMsg ) {

	if ( _dataMsg->getKindOp() == KindOp::DRAW ) {

        glPushMatrix();

        execute ( true, ( Object * ) _dataMsg->getParam() );

        Node::update ( _dataMsg );

        glPopMatrix();

    } else if ( _dataMsg->getKindOp() == KindOp::DRAW_NO_TEX ) {

        glPushMatrix();

        execute ( false, ( Object * ) _dataMsg->getParam() );

        Node::update ( _dataMsg );

        glPopMatrix();

    } else if ( _dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE ) {

        _dataMsg->setDone ( true );

    }
}

btVector3& Object::getPosition() {
    return pPhysic->getPosition();
}

void Object::applyTorc ( const btVector3 &_vet ) {
    if ( pPhysic ) {
        pPhysic->torque ( _vet );
    }
}

void Object::applyForce ( const btVector3 &_vet ) {
    if ( pPhysic ) {
        pPhysic->propulcao ( _vet );
    }
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
