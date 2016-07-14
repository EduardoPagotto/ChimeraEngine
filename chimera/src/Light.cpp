#include "Light.h"

#include <vector>
#include "ChimeraUtils.h"
#include "NodeVisitor.h"

#include "OpenGLDefs.h"

// #ifdef WIN32
// #include "windows.h"
// #endif
// 
// #include <GL/gl.h>
// #include <GL/glu.h>

namespace Chimera {

Light::Light (Node* _parent, std::string _name ) : Node (_parent, EntityKind::LIGHT, _name ) {

    number = 0;
    type = LightType::POSITIONAL;
    ambient = Color::ZERO;
    specular = Color::ZERO;
    diffuse = Color::ZERO;
    position.setZero();

    transform.setIdentity();
}

Light::~Light() {

}

void Light::setPositionRotation ( const btVector3 &_posicao, const btVector3 &_rotation ) {

    //Transformacao quando Euley nao apagar
    btQuaternion l_qtn;
    transform.setIdentity();
    l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
    transform.setRotation ( l_qtn );
    transform.setOrigin ( _posicao );
    //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));
}

void Light::apply(LightNum lightNum) {

    for (std::map<ModeMaterial, void*>::iterator iter = map_params.begin(); iter != map_params.end(); ++iter) {

        ModeMaterial k = iter->first;
        GLfloat *p = (GLfloat*)((Color*)iter->second)->ptr();
        
        glLightfv(lightNum, (GLenum)k, p);
    }
    
    GLfloat posicaoLuz[] = { position.x(), position.y(), position.z(), (type == LightType::POSITIONAL ? 0.0f : 1.0f)  };
    
    glLightfv ( lightNum, GL_POSITION, posicaoLuz );
    glEnable ( lightNum ); 
   
}

void Light::accept(NodeVisitor * v)
{
	v->visit(this);
}

void Light::update(DataMsg *_dataMsg) {

	if (_dataMsg->getKindOp() == KindOp::START) {

		init();
	}

	Node::update(_dataMsg);
}

void Light::init() {

	position = transform.getOrigin();

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
