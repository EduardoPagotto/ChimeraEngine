#include "Light.h"

namespace Chimera {

Light::Light () : Node ( EntityKind::LIGHT) {

    number = 0;
    type = LightType::POINT;
    ambient = Color::ZERO;
    specular = Color::ZERO;
    diffuse = Color::ZERO;
    position.setZero();

}

Light::Light ( const Light& _light ) : Node ( _light ) {

    number = _light.number;
    type = _light.type;
    ambient = _light.ambient;
    specular = _light.specular;
    diffuse = _light.diffuse;

    position = _light.position;
}

Light::~Light() {

}

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
        
        GLfloat posicaoLuz[] = {position.x() , position.y() , position.z() , 1.0f };      
        //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat*)ambient.ptr());
        glLightfv ( testeLuz,GL_AMBIENT, ( GLfloat* ) ambient.ptr() );
        glLightfv ( testeLuz,GL_DIFFUSE, ( GLfloat* ) diffuse.ptr() );
        glLightfv(testeLuz,GL_SPECULAR,(GLfloat*) specular.ptr());

        glLightfv ( testeLuz,GL_POSITION,posicaoLuz );
        glEnable ( testeLuz );
    }
}

void Light::update ( DataMsg *dataMsg ) {
    
    if (dataMsg->getKindOp()==KindOp::START) {
        
        position = transform.getOrigin();
        
    }
    
    Node::update ( dataMsg );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
