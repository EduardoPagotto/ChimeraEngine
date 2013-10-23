#include "Object.h"

namespace Chimera {

Object::Object () : Node ( EntityKind::OBJECT ) {
    
     pPhysic = nullptr;
     pDraw = nullptr;
     pTexture = nullptr;
     pEffect = nullptr;
    
}

Object::Object( const Object& _object ) : Node(_object) {
    
    pPhysic = _object.pPhysic;
    pDraw = _object.pDraw;
    
}

Object::~Object () {
}

void Object::init() {
    
    position = transform.getOrigin();
    
    if (pTexture != nullptr)
        pTexture->init();
    
    if (pDraw!=nullptr)
        pDraw->init();  
    
}

void Object::render() {
    
    if (pEffect != nullptr)
        pEffect->render();
    
    if (pTexture != nullptr)
        pTexture->render();
        
    if (pDraw!=nullptr)
        pDraw->render();
    
}

void Object::update ( DataMsg *_dataMsg ) {
    
    if (_dataMsg->getKindOp()==KindOp::START) {

        init();
        
        //FIXME apenas para teste
        //define propriedades fisicas
        btMaterial *pMat = new btMaterial;
        pMat->m_friction = 0.0f;
        pMat->m_restitution = 0.0f;
        
        Physics *l_pPhysc = new Physics;
        l_pPhysc->setMass(5.0f);
        l_pPhysc->setPhysicMaterial(pMat);
        l_pPhysc->setShapeBox( pDraw->getSizeBox() );
        
        pPhysic = l_pPhysc;
        
        if (pPhysic != nullptr)
            pPhysic->init( transform , getSerial());
                
        Node::update(_dataMsg);
        
    } if (_dataMsg->getKindOp() == KindOp::DRAW3D) {
        
        glPushMatrix();
        
        if (pPhysic!=nullptr) {
            Object *pSource = (Object *)_dataMsg->getParam();
            pPhysic->ajusteMatrix(pSource->pPhysic);
        }     
        
        render();
        
        Node::update(_dataMsg);
       
        glPopMatrix();
     } 
}

void Object::applyTorc ( const btVector3 &_vet ) {
    if ( pPhysic )
        pPhysic->torque ( _vet );
}

void Object::applyForce ( const btVector3 &_vet ) {
    if ( pPhysic )
        pPhysic->propulcao ( _vet );
}

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
