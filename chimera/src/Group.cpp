#include "Group.h"
#include "NodeVisitor.h"

#include "OpenGLDefs.h"

//#include <glm\gtx\inverse_transpose.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp> 

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
        GLint nloc = glGetUniformLocation(idProgram, "noMat");
        
        // Pass the model-view matrix to the shader
        GLfloat mvMat[16]; 
        glGetFloatv(GL_MODELVIEW_MATRIX, mvMat); 
        glUniformMatrix4fv(mvloc, 1, false, mvMat);
    
        // Pass the projection matrix to the shader
        GLfloat pMat[16]; 
        glGetFloatv(GL_PROJECTION_MATRIX, pMat); 
        glUniformMatrix4fv(ploc, 1, false, pMat);
        
        //glm::mat4 modelViewMat;
        //float *ptrMat = glm::value_ptr(modelViewMat);
        //glGetFloatv(GL_MODELVIEW_MATRIX, ptrMat);
        
        if (nloc >= 0) {
            glm::mat4 modelViewMat = glm::make_mat4(mvMat);
            glm::mat3 gl_NormalMatrix = glm::inverseTranspose(glm::mat3(modelViewMat));
            float *pnMat = glm::value_ptr(gl_NormalMatrix);
            glUniformMatrix3fv(nloc, 1, false, pnMat);
        }
        
        
        GLint llumLoc = glGetUniformLocation (idProgram, "l_dir");
        if (llumLoc >= 0) {
         
            Light *pLight = (Light*)findNodeBySeq(EntityKind::LIGHT,0);//FIXME usar outro jeito para pegar esta luz
            if (pLight != nullptr) {
                
                glm::vec3 posicaoLuzZ1 =  pLight->getPosition();
                
                glm::vec3 vLuz(posicaoLuzZ1.x, posicaoLuzZ1.y, posicaoLuzZ1.z);
                float *pvl = glm::value_ptr(vLuz);
                
                glUniform3fv(llumLoc, 1, pvl);
                
            }  
        }
    }
    
    return idProgram;
}
 
 ///
void teste() {
    float pitch;
    float yaw;
    float roll;
    
    glm::vec3 position(0,0,0);
    
    //Euler to Quarterion pitch, yaw, roll angul em radianos
    glm::vec3 myEuler (pitch, yaw, roll);//yaw, pitch, roll ?????
    glm::quat myQuat (myEuler);
    
    glm::mat4 matRot = glm::toMat4(myQuat); //matriz rotacao
    glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), position); //matriz translacao
    glm::mat4 final = matRot * matTrans; //primeiro translada depois rotaciona, ordem é importante!!! 
}
///
 
 
}


