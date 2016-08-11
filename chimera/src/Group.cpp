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
}

Group::~Group() {
}
 
void Group::accept(NodeVisitor* v){
    v->visit(this);
}
 
void Group::apply(const glm::mat4 &_view, const glm::mat4 &_proj) {
    
    if (shader.getIdProgram() > 0) {
        shader.link();
        
        // Get the variables from the shader to which data will be passed
        GLint mvloc = glGetUniformLocation(shader.getIdProgram(), "umvMat");
        GLint ploc = glGetUniformLocation(shader.getIdProgram(), "upMat");
        GLint nloc = glGetUniformLocation(shader.getIdProgram(), "noMat");

		GLint llumLoc = glGetUniformLocation(shader.getIdProgram(), "l_dir");

		glUniformMatrix4fv(mvloc, 1, false, glm::value_ptr(_view));
		glUniformMatrix4fv(ploc, 1, false, glm::value_ptr(_proj));
        
		if (nloc >= 0) {
			glm::mat3 gl_NormalMatrix = glm::inverseTranspose(glm::mat3(_view));
			glUniformMatrix3fv(nloc, 1, false, glm::value_ptr(gl_NormalMatrix));
		}
        
        if (llumLoc >= 0) {
         
            Light *pLight = (Light*)findNodeBySeq(EntityKind::LIGHT,0);//FIXME usar outro jeito para pegar esta luz
            if (pLight != nullptr) {
                
                glm::vec3 posicaoLuzZ1 =  pLight->getPosition();
                
                glm::vec3 vLuz(posicaoLuzZ1.x, posicaoLuzZ1.y, posicaoLuzZ1.z);
                float *pvl = glm::value_ptr(vLuz);
                
                glUniform3fv(llumLoc, 1, pvl);
                
            }  
        }

	} else {

		//openGL 1.4
		//so ate mudar
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(glm::value_ptr(_proj));
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(glm::value_ptr(_view));

	}
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


