#include "Light.h"

#include <vector>
#include "ChimeraUtils.h"
#include "NodeVisitor.h"


#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

Light::Light (Node* _parent, LightType _lightType, std::string _name ) : Node (_parent, EntityKind::LIGHT, _name ) {

    number = 0;
    type = _lightType;
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

void Light::apply() {

    int testeLuz;
    switch ( number ) {
    case 0:
        testeLuz = GL_LIGHT0;
        break;
    case 1:
        testeLuz = GL_LIGHT1;
        break;
    case 2:
        testeLuz = GL_LIGHT2;
        break;
    case 3:
        testeLuz = GL_LIGHT3;
        break;
    case 4:
        testeLuz = GL_LIGHT4;
        break;
    case 5:
        testeLuz = GL_LIGHT5;
        break;
    case 6:
        testeLuz = GL_LIGHT6;
        break;
    case 7:
        testeLuz = GL_LIGHT7;
        break;
    default:
        break;
    }

    GLfloat posicaoLuz[] = { position.x(), position.y(), position.z(), 1.0f };
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat*)ambient.ptr());
    glLightfv ( testeLuz, GL_AMBIENT, ( GLfloat* ) ambient.ptr() );
    glLightfv ( testeLuz, GL_DIFFUSE, ( GLfloat* ) diffuse.ptr() );
    glLightfv ( testeLuz, GL_SPECULAR, ( GLfloat* ) specular.ptr() );

    glLightfv ( testeLuz, GL_POSITION, posicaoLuz );
    glEnable ( testeLuz );
   
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

//TODO colocar no loader
void Light::loadCollada ( tinyxml2::XMLElement* _nNode ) {

    tinyxml2::XMLElement *l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "point" );
    if ( l_nPoint != nullptr ) {

        std::vector<btScalar> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement ( "color" )->GetText();
        loadArrayBtScalar ( l_val, l_arrayF );

        setDiffuse ( Color ( l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f ) );
        return;
    }

    l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "directional" );
    if ( l_nPoint != nullptr ) {

        std::vector<btScalar> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement ( "color" )->GetText();
        loadArrayBtScalar ( l_val, l_arrayF );

        setDiffuse ( Color ( l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f ) );
        return;
    }

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
