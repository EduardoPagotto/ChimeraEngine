#include "Light.h"

namespace Chimera {

Light::Light ( LightType _type, int number, std::string id, std::string _name ) : Node ( EntityType::LIGHT, id, _name ) , typeLight ( _type ), number ( number ) {

    ambient = Color::ZERO;
    specular = Color::ZERO;
    diffuse = Color::ZERO;

}

// Light::Light ( const Light& light ) : Transform ( light ) , typeLight ( light.typeLight ), number ( light.number ) {
// 
// 
//     ambient = light.ambient;
//     specular = light.specular;
//     diffuse = light.diffuse;
// 
// }

Light::~Light() {

}

//void Light::setTransform(const btVector3 &_pos, const btVector3 &_dir)
//{
//	if (m_pPosition==NULL)
//	{
//		m_loader = false;
//		m_pPosition = new btVector3(_pos);
//	}
//
//	if (m_pDirection==NULL)
//		m_pDirection = new btVector3(_dir);
//}

void Light::exec() {
    if ( getStatus() ) {
        int testeLuz;
        switch ( number ) {
        case 0:
            testeLuz=GL_LIGHT0;
            break;
        case 1:
            testeLuz=GL_LIGHT1;
            break;
        case 2:
            testeLuz=GL_LIGHT2;
            break;
        case 3:
            testeLuz=GL_LIGHT3;
            break;
        case 4:
            testeLuz=GL_LIGHT4;
            break;
        case 5:
            testeLuz=GL_LIGHT5;
            break;
        case 6:
            testeLuz=GL_LIGHT6;
            break;
        case 7:
            testeLuz=GL_LIGHT7;
            break;
        default:
            break;
        }

        //TODO Melhorar
        Transform *trans = (Transform*)parent;
        GLfloat posicaoLuz[] = {trans->getPosition().x() , trans->getPosition().y() , trans->getPosition().z() , 1.0f };

        //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat*)ambient.ptr());
        glLightfv ( testeLuz,GL_AMBIENT, ( GLfloat* ) ambient.ptr() );
        glLightfv ( testeLuz,GL_DIFFUSE, ( GLfloat* ) diffuse.ptr() );
        //glLightfv(testeLuz,GL_SPECULAR,(GLfloat*)specular.ptr());

        glLightfv ( testeLuz,GL_POSITION,posicaoLuz );
        glEnable ( testeLuz );
    }
}

void Light::update ( DataMsg *dataMsg ) {
//     if ( _transport->code==DO_INIT ) {
//
//     }
    Node::update ( dataMsg );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
