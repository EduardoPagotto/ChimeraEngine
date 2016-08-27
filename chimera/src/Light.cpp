#include "Light.h"

#include <vector>
#include "ChimeraUtils.h"
#include "NodeVisitor.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

Light::Light (Node* _parent, std::string _name ) : Node (_parent, EntityKind::LIGHT, _name ) {

    number = 0;
    type = LightType::POSITIONAL;
    ambient = Color::ZERO;
    specular = Color::ZERO;
    diffuse = Color::ZERO;
    position = glm::vec3(0.0, 0.0, 0.0);
    transform = glm::mat4(1.0);//transform.setIdentity();

	shader = Singleton<Shader>::getRefSingleton();
}

Light::~Light() {
	Singleton<Shader>::releaseRefSingleton();
}

void Light::setPositionRotation ( const glm::vec3 &_posicao, const glm::vec3 &_rotation ) {

//     //Transformacao quando Euley nao apagar
//     btQuaternion l_qtn;
//     transform.setIdentity();
//     l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
//     transform.setRotation ( l_qtn );
//     transform.setOrigin ( _posicao );
//     //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));
   
    glm::quat myQuat (_rotation); // trocar (pitch, yaw, roll) por (yaw, pitch, roll) ?????
    glm::mat4 matRot = glm::toMat4(myQuat); //matriz rotacao
    glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), _posicao); //matriz translacao
    transform = matRot * matTrans; //primeiro translada depois rotaciona, ordem é importante!!! 
    
}

void Light::apply() {

	shader->setGlUniform3fv("light.position", 1, glm::value_ptr(getPosition()));
	shader->setGlUniform4fv("light.ambient", 1, getAmbient().ptr());
	shader->setGlUniform4fv("light.diffuse", 1, getDiffuse().ptr());
	shader->setGlUniform4fv("light.specular", 1, getSpecular().ptr());

    //for (std::map<ModeMaterial, void*>::iterator iter = map_params.begin(); iter != map_params.end(); ++iter) {

    //    ModeMaterial k = iter->first;
    //    GLfloat *p = (GLfloat*)((Color*)iter->second)->ptr();
    //    
    //    glLightfv(lightNum, (GLenum)k, p);
    //}
    //
    //GLfloat posicaoLuz[] = { position.x, position.y, position.z, (type == LightType::POSITIONAL ? 0.0f : 1.0f)  };
    //
    //glLightfv ( lightNum, GL_POSITION, posicaoLuz );
    //glEnable ( lightNum ); 
   
}

void Light::accept(NodeVisitor * v)
{
	v->visit(this);
}

void Light::init() {

	position = glm::vec3(transform[3]);//position = transform.getOrigin();

}

void Light::loadCollada ( tinyxml2::XMLElement* _nNode ) {

    tinyxml2::XMLElement *l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "point" );
    if ( l_nPoint != nullptr ) {

        type = LightType::POSITIONAL;
        
        std::vector<btScalar> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement ( "color" )->GetText();
        loadArrayBtScalar ( l_val, l_arrayF );

        setDiffuse ( Color ( l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f ) );
        return;
    }

    l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "directional" );
    if ( l_nPoint != nullptr ) {

		type = LightType::DIRECTIONAL;

        std::vector<btScalar> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement ( "color" )->GetText();
        loadArrayBtScalar ( l_val, l_arrayF );

        setDiffuse ( Color ( l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f ) );
        return;
    }

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
