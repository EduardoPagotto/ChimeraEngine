#include "Draw.h"

namespace Chimera {

Draw::Draw() : pMaterial ( nullptr ) , type ( DRAW_TYPE::DRAW_NONE ) {

}

Draw::Draw(const Draw &_draw) {
    
    type = _draw.type;
    pMaterial = _draw.pMaterial;
    
}

Draw::~Draw() {

}

void Draw::init() {
    if ( pMaterial )
        pMaterial->initTex();
}

void Draw::renderizar() {
    
    if ( pMaterial )
        pMaterial->exec();
    else {
        if ( glIsEnabled ( GL_COLOR_MATERIAL ) == 1 ) {
            Color a ( 0.2f, 0.2f, 0.2f, 1.0f );
            Color d ( 0.8f, 0.8f, 0.8f, 1.0f );
            Color s ( 0.0f, 0.0f, 0.0f, 1.0f );
            glMaterialfv ( GL_FRONT, GL_AMBIENT, a.ptr() ); //GL_FRONT_AND_BACK??
            glMaterialfv ( GL_FRONT, GL_DIFFUSE, d.ptr() );
            glMaterialfv ( GL_FRONT, GL_SPECULAR, s.ptr() );
            glMaterialf ( GL_FRONT, GL_SHININESS, 12.0f );
        }
    } 
}

void Draw::ajusteMatrix ( Transform *_pTrans ) {
//	//float *pRotacao = (float*)dBodyGetRotation(m_bodyId);
    //float *pPosicao = (float*)dBodyGetPosition(m_bodyId);
//
//	//btVector3 l_posicaoTeste(pPosicao);
//	float pRotacao[12] = { 0.0f };
//	float pPosicao[3] = { 0.0f };
//
//	GLfloat matrix[16];
//	matrix[0]=pRotacao[0];
//	matrix[1]=pRotacao[4];
//	matrix[2]=pRotacao[8];
//	matrix[3]=0;
//	matrix[4]=pRotacao[1];
//	matrix[5]=pRotacao[5];
//	matrix[6]=pRotacao[9];
//	matrix[7]=0;
//	matrix[8]=pRotacao[2];
//	matrix[9]=pRotacao[6];
//	matrix[10]=pRotacao[10];
//	matrix[11]=0;
//	if (pTemp==0x0)
//	{
//		matrix[12]=pPosicao[0];
//		matrix[13]=pPosicao[1];
//		matrix[14]=pPosicao[2];
//	}
//	else
//	{
//		matrix[12]=pPosicao[0] - pTemp[0];
//		matrix[13]=pPosicao[1] - pTemp[1];
//		matrix[14]=pPosicao[2] - pTemp[2];
//	}
//	matrix[15]=1;
//	glMultMatrixf(matrix);
}

//void Physics::ajusteTridimencional(btVector3 *_vetor)
//{
//	//float *pRotacao = (float*)dBodyGetRotation(m_bodyId);
//	//float *pPosicao = (float*)dBodyGetPosition(m_bodyId);
//	float pRotacao[12] = { 0.0f };
//	float pPosicao[3] = { 0.0f };
//
//	GLfloat matrix[16];
//	matrix[0]=pRotacao[0];
//	matrix[1]=pRotacao[4];
//	matrix[2]=pRotacao[8];
//	matrix[3]=0;
//	matrix[4]=pRotacao[1];
//	matrix[5]=pRotacao[5];
//	matrix[6]=pRotacao[9];
//	matrix[7]=0;
//	matrix[8]=pRotacao[2];
//	matrix[9]=pRotacao[6];
//	matrix[10]=pRotacao[10];
//	matrix[11]=0;
//	if (_vetor==0x0)
//	{
//		matrix[12]=pPosicao[0];
//		matrix[13]=pPosicao[1];
//		matrix[14]=pPosicao[2];
//	}
//	else
//	{
//		matrix[12]=_vetor->x;
//		matrix[13]=_vetor->y;
//		matrix[14]=_vetor->z;
//	}
//	matrix[15]=1;
//	glMultMatrixf (matrix);
//}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
