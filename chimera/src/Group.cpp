#include "Group.h"
#include "NodeVisitor.h"

#include "OpenGLDefs.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

Group::Group(Node* _parent, std::string _name) : Node (_parent, EntityKind::GROUP, _name) {

	shader =  Singleton<Shader>::getRefSingleton();
}

Group::~Group() {

	Singleton<Shader>::releaseRefSingleton();
}

void Group::accept(NodeVisitor* v){
    v->visit(this);
}

void Group::init() {

	shader->selectCurrent(shadeName);

}

void Group::apply(const glm::mat4 &_view, const glm::mat4 &_proj) {

	shader->selectCurrent(shadeName);

    shader->link();

    // Get the variables from the shader to which data will be passed
    shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(_proj));
    shader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(_view));
    //shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr( glm::inverseTranspose(glm::mat3(_view))));

    Light *pLight = (Light*)findChild(EntityKind::LIGHT, 0, false);//(Light*)findNodeBySeq(EntityKind::LIGHT, 0);//FIXME usar outro jeito para pegar esta luz
    if (pLight != nullptr) {

        shader->setGlUniform3fv( "light.position", 1, glm::value_ptr( pLight->getPosition() ) );
        shader->setGlUniform4fv( "light.ambient", 1, pLight->getAmbient().ptr() );
        shader->setGlUniform4fv( "light.diffuse", 1, pLight->getDiffuse().ptr() );
        shader->setGlUniform4fv( "light.specular", 1, pLight->getSpecular().ptr() );

    }

    Camera *pCam = (Camera*)findChild(EntityKind::CAMERA, 0, false);
    if (pCam != nullptr) {

        shader->setGlUniform3fv("viewPos", 1, glm::value_ptr( pCam->getPosition() ) );

    }
}
}


