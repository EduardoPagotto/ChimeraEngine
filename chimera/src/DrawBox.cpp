#include "DrawBox.h"

namespace Chimera {

DrawBox::DrawBox(std::string _id, std::string _name) : Draw(DrawType::BOX,_id,_name) {    
}

DrawBox::DrawBox(const DrawBox &_draw) : Draw(_draw) {
}

DrawBox::~DrawBox() {
}

btVector3 DrawBox::getSizeBox(void) {
    return sizeBox;
}

void DrawBox::init() {
    
}

 void DrawBox::render() {
    
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

void DrawBox::update ( DataMsg *dataMsg ) {
    Node::update(dataMsg);
}

}