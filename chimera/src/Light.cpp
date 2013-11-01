#include "Light.h"

namespace Chimera {

Light::Light (LightType _lightType, std::string _id, std::string _name) : Node ( EntityKind::LIGHT,_id, _name ) {

    number = 0;
    type = _lightType;
    ambient = Color::ZERO;
    specular = Color::ZERO;
    diffuse = Color::ZERO;
    position.setZero();
    
    transform.setIdentity();

}

Light::Light ( const Light& _light ) : Node ( _light ) {

    number = _light.number;
    type = _light.type;
    ambient = _light.ambient;
    specular = _light.specular;
    diffuse = _light.diffuse;

    position = _light.position;
    
    transform = _light.transform;
}

Light::~Light() {

}

void Light::setPositionRotation(btVector3 _posicao, btVector3 _rotation) {

    //Transformacao quando Euley nao apagar
    btQuaternion l_qtn;
    transform.setIdentity();
    l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
    transform.setRotation ( l_qtn );
    transform.setOrigin ( _posicao );
    //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));
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

void Light::clone(Node **ppNode ) {
    *ppNode = new Light( *this ); 
    Node::clone( ppNode );  
}

void Light::update ( DataMsg *dataMsg ) {
    
    if (dataMsg->getKindOp()==KindOp::START) {
        
        position = transform.getOrigin();
        
    }
    
    Node::update ( dataMsg );
}

void Light::loadCollada(tinyxml2::XMLElement* _nNode) {
    
    tinyxml2::XMLElement *l_nPoint = _nNode->FirstChildElement("technique_common")->FirstChildElement("point");
    if (l_nPoint != nullptr) {
        
        std::vector<btScalar> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement("color")->GetText();
        loadArrayBtScalar(l_val, l_arrayF);
        
        setDiffuse( Color(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f) );
        
    } 
    
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
