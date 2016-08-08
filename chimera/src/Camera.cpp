#include "Camera.h"
#include "NodeVisitor.h"


//#include <glm/gtc/matrix_inverse.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {
    
Camera::Camera (Node* _pNode, CameraType _type, std::string _name )
	: type(_type), Node(_pNode, EntityKind::CAMERA, _name ) {

    type = _type;
    position = glm::vec3(0.0,0.0,0.0);// setZero();
    rotation = glm::vec3(0.0,0.0,0.0);//.setZero();
    direction = glm::vec3(0.0,0.0,0.0);//.setZero();
    transform = glm::mat4(1.0);//transform.setIdentity();

    nearDistance = 0.1f;
    farDistance = 1000.0f;
    fov = 45.0f;

    perspective = true;
}

Camera::~Camera() {
}

void Camera::setPositionRotation ( const glm::vec3 &_posicao, const glm::vec3 &_rotation ) {

    //Transformacao quando Euley nao apagar
//     btQuaternion l_qtn;
//     transform.setIdentity();
//     l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
//     transform.setRotation ( l_qtn );
//     transform.setOrigin ( _posicao );
     
    //Euler to Quarterion pitch, yaw, roll angul em radianos
    glm::quat myQuat (_rotation); // trocar (pitch, yaw, roll) por (yaw, pitch, roll) ?????
    
    glm::mat4 matRot = glm::toMat4(myQuat); //matriz rotacao
    glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), _posicao); //matriz translacao
    transform = matRot * matTrans; //primeiro translada depois rotaciona, ordem é importante!!! 
    
}


void Camera::render ( void ) {
 
    //         btScalar m_matrix[16];
    //         btTransform l_transform;
    //         btQuaternion l_qpitch;
    //
    //         l_transform.setIdentity();
    //
    //         l_qpitch.setEuler( -(direction.getZ() * SIMD_RADS_PER_DEG) ,
    //                            -(direction.getX() * SIMD_RADS_PER_DEG) ,
    //                            -(direction.getY() * SIMD_RADS_PER_DEG));
    //
    //         btVector3 l_tempz1(-position.getX() , -position.getZ(), position.getY() );
    //         l_transform.setOrigin(l_tempz1);
    //         l_transform.setRotation(l_qpitch);
    //         l_transform.getOpenGLMatrix(m_matrix);
    //
    //         glMultMatrixf(m_matrix);


    gluLookAt ( position.x, position.y, position.z,
                direction.x, direction.y, direction.z,
                rotation.x, rotation.y, rotation.z );
}

void Camera::accept(NodeVisitor * v) {
	v->visit(this);
}

void Camera::init() {

   
    position = glm::vec3(transform[3]);//position = transform.getOrigin();
    direction = glm::vec3 ( 0.0, 0.0, 0.0 ); //FIXME encontrar no transform
    rotation = glm::vec3 ( 0.0, 0.0, 1.0 ); //FIXME encontrar no transform

}

//TODO criar classe de loader
void Camera::loadCollada ( tinyxml2::XMLElement* _nNode ) {

    tinyxml2::XMLElement *l_nPerspective = _nNode->FirstChildElement ( "optics" )->FirstChildElement ( "technique_common" )->FirstChildElement ( "perspective" );
    if ( l_nPerspective != nullptr ) {

        setPerspective ( true );
        setFov ( atof ( l_nPerspective->FirstChildElement ( "xfov" )->GetText() ) );
        setNear ( atof ( l_nPerspective->FirstChildElement ( "znear" )->GetText() ) );
        setFar ( atof ( l_nPerspective->FirstChildElement ( "zfar" )->GetText() ) );

    } else {
        //TODO testar ecarregar ortogonal aqui

    }

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
