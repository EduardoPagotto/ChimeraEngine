#include "Object.h"

namespace Chimera {

Object::Object () : Node ( EntityType::OBJECT ) {
    
    m_pPhysic = nullptr;
    m_pDraw = nullptr;
    
}

Object::Object( const Object& object ) : Node(object) {
    
    m_pPhysic = object.m_pPhysic;
    m_pDraw = object.m_pDraw;
    
}

Object::~Object ( void ) {
}

void Object::update ( DataMsg *dataMsg ) {
//FIXME mais um
    if (dataMsg->getKindOperation() == KindOperation::DRAW3D) {
        
 		glPushMatrix();

		if (m_pPhysic) {
// 			Object *pSource = (Object *)_transport->fieldA;
// 			m_pPhysic->ajusteMatrix(pSource->m_pPhysic);
		}

 		if (m_pDraw)
 			m_pDraw->renderizar();

 		glPopMatrix();

        Node::update(dataMsg);

 		//glPopMatrix();

     }
// 	else if (_transport->code==DO_INIT)
// 	{
// 		if (m_pDraw)
// 			m_pDraw->init();
//
// 		if (m_pPhysic)
// 			m_pPhysic->init(&m_trans);
//
// 		Node::update(_transport);
// 	}
}

void Object::applyTorc ( const btVector3 &_vet ) {
    if ( m_pPhysic )
        m_pPhysic->torque ( _vet );
}

void Object::applyForce ( const btVector3 &_vet ) {
    if ( m_pPhysic )
        m_pPhysic->propulcao ( _vet );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
