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
//void Draw::ajusteMatrix ( Transform *_pTrans ) {
//  //float *pRotacao = (float*)dBodyGetRotation(m_bodyId);
//float *pPosicao = (float*)dBodyGetPosition(m_bodyId);
//
//  //btVector3 l_posicaoTeste(pPosicao);
//  float pRotacao[12] = { 0.0f };
//  float pPosicao[3] = { 0.0f };
//
//  GLfloat matrix[16];
//  matrix[0]=pRotacao[0];
//  matrix[1]=pRotacao[4];
//  matrix[2]=pRotacao[8];
//  matrix[3]=0;
//  matrix[4]=pRotacao[1];
//  matrix[5]=pRotacao[5];
//  matrix[6]=pRotacao[9];
//  matrix[7]=0;
//  matrix[8]=pRotacao[2];
//  matrix[9]=pRotacao[6];
//  matrix[10]=pRotacao[10];
//  matrix[11]=0;
//  if (pTemp==0x0)
//  {
//      matrix[12]=pPosicao[0];
//      matrix[13]=pPosicao[1];
//      matrix[14]=pPosicao[2];
//  }
//  else
//  {
//      matrix[12]=pPosicao[0] - pTemp[0];
//      matrix[13]=pPosicao[1] - pTemp[1];
//      matrix[14]=pPosicao[2] - pTemp[2];
//  }
//  matrix[15]=1;
//  glMultMatrixf(matrix);
//}

//void Physics::ajusteTridimencional(btVector3 *_vetor)
//{
//  //float *pRotacao = (float*)dBodyGetRotation(m_bodyId);
//  //float *pPosicao = (float*)dBodyGetPosition(m_bodyId);
//  float pRotacao[12] = { 0.0f };
//  float pPosicao[3] = { 0.0f };
//
//  GLfloat matrix[16];
//  matrix[0]=pRotacao[0];
//  matrix[1]=pRotacao[4];
//  matrix[2]=pRotacao[8];
//  matrix[3]=0;
//  matrix[4]=pRotacao[1];
//  matrix[5]=pRotacao[5];
//  matrix[6]=pRotacao[9];
//  matrix[7]=0;
//  matrix[8]=pRotacao[2];
//  matrix[9]=pRotacao[6];
//  matrix[10]=pRotacao[10];
//  matrix[11]=0;
//  if (_vetor==0x0)
//  {
//      matrix[12]=pPosicao[0];
//      matrix[13]=pPosicao[1];
//      matrix[14]=pPosicao[2];
//  }
//  else
//  {
//      matrix[12]=_vetor->x;
//      matrix[13]=_vetor->y;
//      matrix[14]=_vetor->z;
//  }
//  matrix[15]=1;
//  glMultMatrixf (matrix);
//}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
