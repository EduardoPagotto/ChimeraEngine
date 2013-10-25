#include "Object.h"

namespace Chimera {

Object::Object () : Node ( EntityKind::OBJECT ) {
    
     pPhysic = nullptr;
     pDraw = nullptr;
     pTexture = nullptr;
     pEffect = nullptr;
     
     collide = false;
     
     logger = log4cxx::Logger::getLogger ( "Object" );
    
}

Object::Object( const Object& _object ) : Node(_object) {
    
    pPhysic = _object.pPhysic;
    pDraw = _object.pDraw;
    
}

Object::~Object () {
}

void Object::init() {
        
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
        
        if (pPhysic == nullptr) {
        
            //FIXME apenas para teste define propriedades fisicas
            pPhysic = new Physics;
            pPhysic->setMass(5.0f);
            pPhysic->setFriction(0.0f);
            pPhysic->setRestitution(0.0f);
                        
        }
         
        if (pDraw != nullptr)
            pPhysic->setShapeBox( pDraw->getSizeBox() );
         
        pPhysic->init( transform , this);
                
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
     } if (_dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE) {
         
         _dataMsg->setDone(true);
         
     }
}

btVector3& Object::getPosition() {
    return pPhysic->getPosition();
}

void Object::applyTorc ( const btVector3 &_vet ) {
    if ( pPhysic )
        pPhysic->torque ( _vet );
}

void Object::applyForce ( const btVector3 &_vet ) {
    if ( pPhysic )
        pPhysic->propulcao ( _vet );
}

// bool Object::get_check_collision() {
//      
//     std::string l_msg = "obj " + getId() + " " + "func: get_check_collision";
//     
//     LOG4CXX_INFO ( logger , l_msg );
//     
//      return true;
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
