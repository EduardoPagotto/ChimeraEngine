#include "Object.h"

namespace Chimera {

Object::Object ( std::string name ) : m_pPhysic ( NULL ) , m_pDraw ( NULL ), Transform ( EntityType::OBJECT,name ) {
    
}

Object::~Object ( void ) {

}

void Object::update ( DataMsg *dataMsg ) {
//FIXME mais um
// 	if (_transport->code==DO_DRAW_3D)
//     {
// 		glPushMatrix();
//
// 		if (m_pPhysic)
// 		{
// 			Object *pSource = (Object *)_transport->fieldA;
// 			m_pPhysic->ajusteMatrix(pSource->m_pPhysic);
// 		}
//
// 		if (m_pDraw)
// 			m_pDraw->render();
//
// 		glPopMatrix();
//
// 		Node::update(_transport);
//
// 		//glPopMatrix();
//
//     }
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
