#include "DrawBox.h"

namespace Chimera {

DrawBox::DrawBox() : Draw(DrawType::BOX) {    
}

DrawBox::DrawBox(const DrawBox &_draw) : Draw(_draw) {
}

DrawBox::~DrawBox() {
}

btVector3 DrawBox::getSizeBox(void) {
    return sizeBox;
}

void DrawBox::update ( DataMsg *dataMsg ) {
    
    Draw::update(dataMsg);
    
    if (dataMsg->getKindOp()==KindOp::START) {

    } else if (dataMsg->getKindOp()==KindOp::DRAW3D) {
        
        glBegin(GL_QUADS);          // Face posterior
        glNormal3f(0.0, 0.0, 1.0);  // Normal da face
        glVertex3f(sizeBox.getX(), sizeBox.getY(), sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), sizeBox.getY(), sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), -sizeBox.getY(), sizeBox.getZ());
        glVertex3f(sizeBox.getX(), -sizeBox.getY(), sizeBox.getZ());
        glEnd();
        glBegin(GL_QUADS);          // Face frontal
        glNormal3f(0.0, 0.0, -1.0);     // Normal da face
        glVertex3f(sizeBox.getX(), sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(sizeBox.getX(), -sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), -sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), sizeBox.getY(), -sizeBox.getZ());
        glEnd();
        glBegin(GL_QUADS);          // Face lateral esquerda
        glNormal3f(-1.0, 0.0, 0.0);     // Normal da face
        glVertex3f(-sizeBox.getX(), sizeBox.getY(), sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), -sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), -sizeBox.getY(), sizeBox.getZ());
        glEnd();
        glBegin(GL_QUADS);          // Face lateral direita
        glNormal3f(1.0, 0.0, 0.0);  // Normal da face
        glVertex3f(sizeBox.getX(), sizeBox.getY(), sizeBox.getZ());
        glVertex3f(sizeBox.getX(), -sizeBox.getY(), sizeBox.getZ());
        glVertex3f(sizeBox.getX(), -sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(sizeBox.getX(), sizeBox.getY(), -sizeBox.getZ());
        glEnd();
        glBegin(GL_QUADS);          // Face superior
        glNormal3f(0.0, 1.0, 0.0);      // Normal da face
        glVertex3f(-sizeBox.getX(), sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), sizeBox.getY(), sizeBox.getZ());
        glVertex3f(sizeBox.getX(), sizeBox.getY(), sizeBox.getZ());
        glVertex3f(sizeBox.getX(), sizeBox.getY(), -sizeBox.getZ());
        glEnd();
        glBegin(GL_QUADS);          // Face inferior
        glNormal3f(0.0, -1.0, 0.0);     // Normal da face
        glVertex3f(-sizeBox.getX(), -sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(sizeBox.getX(), -sizeBox.getY(), -sizeBox.getZ());
        glVertex3f(sizeBox.getX(), -sizeBox.getY(), sizeBox.getZ());
        glVertex3f(-sizeBox.getX(), -sizeBox.getY(), sizeBox.getZ());
        glEnd();       
    
    }   
}

}