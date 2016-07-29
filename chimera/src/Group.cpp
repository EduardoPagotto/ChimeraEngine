#include "Group.h"
#include "NodeVisitor.h"

#include "OpenGLDefs.h"

namespace Chimera {

Group::Group(Node* _parent, std::string _name) : Node (_parent, EntityKind::GROUP, _name) { 
	idProgram = 0;
}

Group::~Group() {
}
 
void Group::accept(NodeVisitor* v){
    v->visit(this);
}
 
void Group::setIdProgram(const GLuint &_id) {
    if (_id > 0)
        idProgram = _id;
} 
 
GLuint Group::applyIdProgram() {
    
    if (idProgram > 0) {
        
        glUseProgram(idProgram);
        
        // Get the variables from the shader to which data will be passed
        GLint mvloc = glGetUniformLocation(idProgram, "umvMat");
        GLint ploc = glGetUniformLocation(idProgram, "upMat");
    
        // Pass the model-view matrix to the shader
        GLfloat mvMat[16]; 
        glGetFloatv(GL_MODELVIEW_MATRIX, mvMat); 
        glUniformMatrix4fv(mvloc, 1, false, mvMat);
    
        // Pass the projection matrix to the shader
        GLfloat pMat[16]; 
        glGetFloatv(GL_PROJECTION_MATRIX, pMat); 
        glUniformMatrix4fv(ploc, 1, false, pMat);
        
    }
    
    return idProgram;
}
 
}


