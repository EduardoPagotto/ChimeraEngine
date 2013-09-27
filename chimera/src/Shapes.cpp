#include "Shapes.h"

namespace Chimera {
	
DrawBox::DrawBox(float _lado, float _altura, float _largura) : m_lado(_lado), m_altura(_altura), m_largura(_largura)
{
	m_typeDraw = BOX;
}

DrawBox::~DrawBox(){}

btVector3 DrawBox::sizeQuadratic(void)
{
	return btVector3( m_lado, m_largura, m_altura );
}

void DrawBox::renderizar()
{
	glBegin(GL_QUADS);			// Face posterior
		glNormal3f(0.0, 0.0, 1.0);	// Normal da face
		glVertex3f(m_lado, m_altura, m_largura);
		glVertex3f(-m_lado, m_altura, m_largura);
		glVertex3f(-m_lado, -m_altura, m_largura);
		glVertex3f(m_lado, -m_altura, m_largura);
	glEnd();
	glBegin(GL_QUADS);			// Face frontal
		glNormal3f(0.0, 0.0, -1.0); 	// Normal da face
		glVertex3f(m_lado, m_altura, -m_largura);
		glVertex3f(m_lado, -m_altura, -m_largura);
		glVertex3f(-m_lado, -m_altura, -m_largura);
		glVertex3f(-m_lado, m_altura, -m_largura);
	glEnd();
	glBegin(GL_QUADS);			// Face lateral esquerda
		glNormal3f(-1.0, 0.0, 0.0); 	// Normal da face
		glVertex3f(-m_lado, m_altura, m_largura);
		glVertex3f(-m_lado, m_altura, -m_largura);
		glVertex3f(-m_lado, -m_altura, -m_largura);
		glVertex3f(-m_lado, -m_altura, m_largura);
	glEnd();
	glBegin(GL_QUADS);			// Face lateral direita
		glNormal3f(1.0, 0.0, 0.0);	// Normal da face
		glVertex3f(m_lado, m_altura, m_largura);
		glVertex3f(m_lado, -m_altura, m_largura);
		glVertex3f(m_lado, -m_altura, -m_largura);
		glVertex3f(m_lado, m_altura, -m_largura);
	glEnd();
	glBegin(GL_QUADS);			// Face superior
		glNormal3f(0.0, 1.0, 0.0);  	// Normal da face
		glVertex3f(-m_lado, m_altura, -m_largura);
		glVertex3f(-m_lado, m_altura, m_largura);
		glVertex3f(m_lado, m_altura, m_largura);
		glVertex3f(m_lado, m_altura, -m_largura);
	glEnd();
	glBegin(GL_QUADS);			// Face inferior
		glNormal3f(0.0, -1.0, 0.0); 	// Normal da face
		glVertex3f(-m_lado, -m_altura, -m_largura);
		glVertex3f(m_lado, -m_altura, -m_largura);
		glVertex3f(m_lado, -m_altura, m_largura);
		glVertex3f(-m_lado, -m_altura, m_largura);
	glEnd();
}

//-----------------------------------------------------

DrawBoxGrid::DrawBoxGrid(int _size,int _space) : m_size(_size), m_space(_space)
{
	m_typeDraw = BOXGRID;

	m_size = 50;
	m_space = 5;
}

DrawBoxGrid::~DrawBoxGrid()
{

}

btVector3 DrawBoxGrid::sizeQuadratic(void)
{
	btScalar l_val = m_size / 2.f;
	return btVector3( l_val, l_val, l_val );
}

void DrawBoxGrid::renderizar()
{
	int px = m_size;
	int py = m_size;
	int pz = m_size;

	for (pz = -m_size ; pz <= m_size ; pz+=m_space)
	{
		glColor3ub(255,0,0);
		glBegin(GL_LINE_LOOP);
			glVertex3i(px, py, pz);
			glVertex3i(px, -py, pz);
			glVertex3i(-px, -py, pz);
			glVertex3i(-px, py, pz);
		glEnd();

		glColor3ub(0,255,0);
		glBegin(GL_LINE_LOOP);
			glVertex3i(pz, py, px);
			glVertex3i(pz, py, -px);
			glVertex3i(pz, -py, -px);
			glVertex3i(pz, -py, px);
		glEnd();

		glColor3ub(0,0,255);
		glBegin(GL_LINE_LOOP);
			glVertex3i(px, pz, py);
			glVertex3i(-px, pz, py);
			glVertex3i(-px, pz, -py);
			glVertex3i(px, pz, -py);
		glEnd();
	}
}

//--------------------------------------------------------

DrawBoxGrid2::DrawBoxGrid2()
{
	m_typeDraw = BOXGRID2;
}

DrawBoxGrid2::~DrawBoxGrid2()
{

}

btVector3 DrawBoxGrid2::sizeQuadratic(void)
{
	return btVector3( 10.0f, 10.0f, 10.0f );
}

void DrawBoxGrid2::renderizar()
{
	#define MAX_LINE 1000
	#define DELTA_LINE 50
	#define STEP_LINE 10

	float maxY = 20;//fmod(_posicao.y,DELTA_LINE);
	float maxZ = 20;//fmod(_posicao.z,DELTA_LINE);
	float maxX = 20;//fmod(_posicao.x,DELTA_LINE);

	glColor3ub(225,0,0);
	glBegin(GL_LINES);
	for (float K=-2;K<=2;K++)
		for (float J=-2;J<=2;J++)
		{
			glVertex3f(-MAX_LINE,maxY+(DELTA_LINE*K),maxZ+(DELTA_LINE*J));
			glVertex3f(MAX_LINE,maxY+(DELTA_LINE*K),maxZ+(DELTA_LINE*J));

			glVertex3f(maxX+(DELTA_LINE*J),maxY+(DELTA_LINE*K),-MAX_LINE);
			glVertex3f(maxX+(DELTA_LINE*J),maxY+(DELTA_LINE*K),MAX_LINE);

			glVertex3f(maxX+(DELTA_LINE*K),-MAX_LINE,maxZ+(DELTA_LINE*J));
			glVertex3f(maxX+(DELTA_LINE*K),MAX_LINE,maxZ+(DELTA_LINE*J));
		}
	glEnd();
}

//
//void ajusteMatrizDesenho(btVector3 _posicao)
//{
//	//float *pRotacao = (float*)dBodyGetRotation(m_bodyId);
//	//float *pPosicao = (float*)dBodyGetPosition(m_bodyId);
//
//	//btVector3 l_posicaoTeste(pPosicao);
//
//	GLfloat mrot[9] = { 0.0f, 0.0f, 0.0f, 
//						0.0f, 0.0f, 0.0f,
//						0.0f, 0.0f, 0.0f};
//	GLfloat matrix[16];
//	
//	matrix[0] = mrot[0];
//	matrix[1] = mrot[1];
//	matrix[2] = mrot[2];
//	matrix[3] = 0;
//	matrix[4] = mrot[3];
//	matrix[5] = mrot[4];
//	matrix[6] = mrot[5];
//	matrix[7] = 0;
//	matrix[8] = mrot[6];
//	matrix[9] = mrot[7];
//	matrix[10] = mrot[8];
//	matrix[11] = 0;
//
//	matrix[12] = _posicao.x;
//	matrix[13] = _posicao.y;
//	matrix[14] = _posicao.z;
//	
//	matrix[15] = 1;
//
//	glMultMatrixf (matrix);
//}

}
