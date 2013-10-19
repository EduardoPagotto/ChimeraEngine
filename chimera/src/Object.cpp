#include "Object.h"

namespace Chimera {

Object::Object () : Node ( EntityKind::OBJECT ) {
    
//     pPhysic = nullptr;
     pDraw = nullptr;
     pTexture = nullptr;
     pEffect = nullptr;
    
}

Object::Object( const Object& _object ) : Node(_object) {
    
//    pPhysic = _object.pPhysic;
    pDraw = _object.pDraw;
    
}

Object::~Object () {
}

void Object::update ( DataMsg *_dataMsg ) {
//FIXME mais um
    
    if (_dataMsg->getKindOp()==KindOp::START) {

        position = transform.getOrigin();
       
        if (pTexture != nullptr)
            pTexture->init();
       
        if (pDraw!=nullptr)
            pDraw->init();
        
        Node::update(_dataMsg);
        
        //      if (m_pDraw)
        //          m_pDraw->init();
        //
        //      if (m_pPhysic)
        //          m_pPhysic->init(&m_trans);
        //
        //      Node::update(_transport);
        
    } if (_dataMsg->getKindOp() == KindOp::DRAW3D) {
        
        glPushMatrix();
        
        if (pTexture != nullptr)
            pTexture->render();
        
        if (pEffect != nullptr)
            pEffect->render();
        
        if (pDraw!=nullptr)
            pDraw->render();       
              
        Node::update(_dataMsg);
        
        glPopMatrix();
        
        
//         glPushMatrix();
// 
//         if (pPhysic) {
//             Object *pSource = (Object *)_transport->fieldA;
//             m_pPhysic->ajusteMatrix(pSource->m_pPhysic);
//         }
// 
//         if (pDraw)
//         pDraw->renderizar();
// 
//         glPopMatrix();
// 
//         Node::update(_dataMsg);
// 
//         glPopMatrix();

     } 
}

// void Object::applyTorc ( const btVector3 &_vet ) {
//     if ( pPhysic )
//         pPhysic->torque ( _vet );
// }
// 
// void Object::applyForce ( const btVector3 &_vet ) {
//     if ( pPhysic )
//         pPhysic->propulcao ( _vet );
// }

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
