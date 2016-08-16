#include "Group.h"
#include "NodeVisitor.h"

#include "OpenGLDefs.h"

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
        shader.setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(_proj));
        shader.setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(_view));
        //shader.setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr( glm::inverseTranspose(glm::mat3(_view))));

		Light *pLight = (Light*)findNodeBySeq(EntityKind::LIGHT, 0);//FIXME usar outro jeito para pegar esta luz
		if (pLight != nullptr) {
            
            shader.setGlUniform3fv( "lightPos", 1, glm::value_ptr( pLight->getPosition() ) );
            shader.setGlUniform4fv( "light.ambient", 1, pLight->getAmbient().ptr() );
			shader.setGlUniform4fv( "light.diffuse", 1, pLight->getDiffuse().ptr() );
			shader.setGlUniform4fv( "light.specular", 1, pLight->getSpecular().ptr() );

		}

		Camera *pCam = (Camera*)findNodeBySeq(EntityKind::CAMERA, 0);
		if (pCam != nullptr) {

            shader.setGlUniform3fv("viewPos", 1, glm::value_ptr( pCam->getPosition() ) );
           
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
// void teste() {
//     float pitch;
//     float yaw;
//     float roll;
//     
//     glm::vec3 position(0,0,0);
//     
//     //Euler to Quarterion pitch, yaw, roll angul em radianos
//     glm::vec3 myEuler (pitch, yaw, roll);//yaw, pitch, roll ?????
//     glm::quat myQuat (myEuler);
//     
//     glm::mat4 matRot = glm::toMat4(myQuat); //matriz rotacao
//     glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), position); //matriz translacao
//     glm::mat4 final = matRot * matTrans; //primeiro translada depois rotaciona, ordem é importante!!! 
// }
///
 
 
}


