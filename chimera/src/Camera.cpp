#include "Camera.h"

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

 Camera::Camera (CameraType _type, std::string _id, std::string _name) : Node ( EntityKind::CAMERA,_id, _name ) {
    
    type = _type;
    position.setZero();
    rotation.setZero();
    direction.setZero();
    transform.setIdentity();
    
    nearDistance = 0.1f;
    farDistance = 1000.0f;
    fov = 45.0f;
   
    perspective = true; 
}

Camera::Camera (const Camera& _camera ) : Node ( _camera ) {
    
    type = _camera.type;
    nearDistance = _camera.nearDistance;
    farDistance = _camera.farDistance;
    fov = _camera.fov;
    
    position = _camera.position;
    rotation = _camera.rotation;
    direction = _camera.direction;
    
    transform = _camera.transform;
    
    perspective = _camera.perspective;
}

Camera::~Camera() {
}

void Camera::clone(Node **ppNode ) {
    *ppNode = new Camera( *this ); 
    Node::clone( ppNode );  
}
    
void Camera::setPositionRotation(const btVector3 &_posicao, const btVector3 &_rotation) {

    //Transformacao quando Euley nao apagar
    btQuaternion l_qtn;
    transform.setIdentity();
    l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
    transform.setRotation ( l_qtn );
    transform.setOrigin ( _posicao );
    //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));
  
}
    

void Camera::exec ( void ) {
    if ( getStatus() ==true ) {
        
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
            
        
        gluLookAt ( position.x(), position.y(), position.z(),
                    direction.x(), direction.y(), direction.z(),
                    rotation.x(), rotation.y(), rotation.z() );

    }
}

void Camera::init() {
    
    position = transform.getOrigin();        
    direction.setValue(0.0, 0.0, 0.0);//FIXME encontrar no transform
    rotation.setValue(0.0, 0.0, 1.0);//FIXME encontrar no transform
    
}

void Camera::update ( DataMsg *_dataMsg ) {
    
    if  (_dataMsg->getKindOp() == KindOp::START) {
  
        init();
             
    } if (_dataMsg->getKindOp() == KindOp::DRAW3D) {
        
    } 
    
    Node::update ( _dataMsg );
}

//TODO criar classe de loader
void Camera::loadCollada(tinyxml2::XMLElement* _nNode) {
    
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
