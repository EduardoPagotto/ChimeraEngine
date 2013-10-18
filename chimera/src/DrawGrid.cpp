#include "DrawGrid.h"

namespace Chimera {


DrawGrid::DrawGrid () : Draw(DrawType::GRID)  {   
}

DrawGrid::DrawGrid(const DrawGrid &_draw) : Draw(_draw) {   
}

DrawGrid::~DrawGrid() {
}

btVector3 DrawGrid::getSizeBox(void) {
    return sizeBox;
}

void DrawGrid::update ( DataMsg *dataMsg ) {
    
    Draw::update(dataMsg);
    
    if (dataMsg->getKindOp()==KindOp::START) {
    
//         if ( pMaterial )
//             pMaterial->initTex();
        
    } else if (dataMsg->getKindOp()==KindOp::DRAW3D) {
        
        btScalar px = sizeBox.getX();
        btScalar py = sizeBox.getY();
        btScalar pz = -sizeBox.getZ();
        
        do {
            
            glColor3f ( colorX.r, colorX.g , colorX.b );//glColor3ub ( 255,0,0 );
            glBegin ( GL_LINE_LOOP );
            glVertex3f ( px, py, pz );
            glVertex3f ( px, -py, pz );
            glVertex3f ( -px, -py, pz );
            glVertex3f ( -px, py, pz );
            glEnd();
            
            glColor3f ( colorX.r, colorX.g , colorX.b );//glColor3ub ( 0,255,0 );
            glBegin ( GL_LINE_LOOP );
            glVertex3f ( pz, py, px );
            glVertex3f ( pz, py, -px );
            glVertex3f ( pz, -py, -px );
            glVertex3f ( pz, -py, px );
            glEnd();
            
            glColor3f ( colorX.r, colorX.g , colorX.b );//glColor3ub ( 0,0,255 );
            glBegin ( GL_LINE_LOOP );
            glVertex3f ( px, pz, py );
            glVertex3f ( -px, pz, py );
            glVertex3f ( -px, pz, -py );
            glVertex3f ( px, pz, -py );
            glEnd();
            
            pz += space;
            
        } while (pz < sizeBox.getZ());
    }
}

// void DrawBoxGrid2::renderizar() {
//  #define MAX_LINE 1000
//  #define DELTA_LINE 50
//  #define STEP_LINE 10
// 
//  float maxY = 20;//fmod(_posicao.y,DELTA_LINE);
//  float maxZ = 20;//fmod(_posicao.z,DELTA_LINE);
//  float maxX = 20;//fmod(_posicao.x,DELTA_LINE);
// 
//  Draw::renderizar();
//  
//  glColor3ub(225,0,0);
//  glBegin(GL_LINES);
//  for (float K=-2;K<=2;K++) {
//      for (float J=-2;J<=2;J++) {
//          glVertex3f(-MAX_LINE,maxY+(DELTA_LINE*K),maxZ+(DELTA_LINE*J));
//          glVertex3f(MAX_LINE,maxY+(DELTA_LINE*K),maxZ+(DELTA_LINE*J));
// 
//          glVertex3f(maxX+(DELTA_LINE*J),maxY+(DELTA_LINE*K),-MAX_LINE);
//          glVertex3f(maxX+(DELTA_LINE*J),maxY+(DELTA_LINE*K),MAX_LINE);
// 
//          glVertex3f(maxX+(DELTA_LINE*K),-MAX_LINE,maxZ+(DELTA_LINE*J));
//          glVertex3f(maxX+(DELTA_LINE*K),MAX_LINE,maxZ+(DELTA_LINE*J));
//      }
//     }
//  glEnd();
// }

//
//void ajusteMatrizDesenho(btVector3 _posicao)
//{
//  //float *pRotacao = (float*)dBodyGetRotation(m_bodyId);
//  //float *pPosicao = (float*)dBodyGetPosition(m_bodyId);
//
//  //btVector3 l_posicaoTeste(pPosicao);
//
//  GLfloat mrot[9] = { 0.0f, 0.0f, 0.0f, 
//                      0.0f, 0.0f, 0.0f,
//                      0.0f, 0.0f, 0.0f};
//  GLfloat matrix[16];
//  
//  matrix[0] = mrot[0];
//  matrix[1] = mrot[1];
//  matrix[2] = mrot[2];
//  matrix[3] = 0;
//  matrix[4] = mrot[3];
//  matrix[5] = mrot[4];
//  matrix[6] = mrot[5];
//  matrix[7] = 0;
//  matrix[8] = mrot[6];
//  matrix[9] = mrot[7];
//  matrix[10] = mrot[8];
//  matrix[11] = 0;
//
//  matrix[12] = _posicao.x;
//  matrix[13] = _posicao.y;
//  matrix[14] = _posicao.z;
//  
//  matrix[15] = 1;
//
//  glMultMatrixf (matrix);
//}

}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
