#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>

#include "Object.h"
#include "NodeVisitor.h"

namespace Chimera {
    
Object::Object (Node* _parent, std::string _name ) : Group( _parent , _name ) {

	setKind(EntityKind::OBJECT);

    pPhysic = nullptr;
    transform.setIdentity();
}

Object::~Object() {
}

void Object::setPositionRotation ( const btVector3 &_posicao, const btVector3 &_rotation ) {

    btQuaternion l_qtn;
    transform.setIdentity();
    l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
    transform.setRotation ( l_qtn );
	transform.setOrigin(_posicao);
    //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));
}

void Object::init() {
    
	Draw *pDraw = (Draw*)Node::findChildByKind(EntityKind::DRAW, 0);

	if (pPhysic->isShapeDefine() == false)
		pPhysic->setShapeBox(pDraw->getSizeBox());

	pPhysic->initTransform(transform, this);
	
	Node::init();
}

void Object::accept(NodeVisitor * v) {
	v->visit(this);
}

void Object::update ( DataMsg *_dataMsg ) {

	if (( _dataMsg->getKindOp() == KindOp::DRAW ) || (_dataMsg->getKindOp() == KindOp::DRAW_NO_TEX)) {

        glPushMatrix();

		Object *pSource = (Object *)_dataMsg->getParam();
		pPhysic->ajusteMatrix(pSource->pPhysic);
		
        Node::update ( _dataMsg );

        glPopMatrix();

    } else if ( _dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE ) {

        _dataMsg->setDone ( true );
		//Node::update(_dataMsg);
    }
}

btVector3& Object::getPosition() {
    return pPhysic->getPosition();
}

void Object::applyTorc ( const btVector3 &_vet ) {
    pPhysic->torque ( _vet );  
}

void Object::applyForce ( const btVector3 &_vet ) {
    pPhysic->propulcao ( _vet );
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
