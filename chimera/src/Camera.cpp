#include "Camera.h"

namespace Chimera {

Camera::Camera ( CameraType _camType, std::string id, std::string name ) : Transform ( EntityType::CAMERA, id, name ), cameraType ( _camType ) {
    
    near = 0.1f;
    far = 1000.0f;
    fov = 45.0f;
 
}

Camera::Camera (const Camera& camera ) : Transform ( camera ), cameraType ( camera.cameraType ) {
    
    near = camera.near;
    far = camera.far;
    fov = camera.fov;
    
}

Camera::~Camera() {
}

void Camera::exec ( void ) {
    if ( getStatus() ==true ) {
        //btScalar m_matrix[16];

        //btTransform transform;
        //btQuaternion l_qPitch;

        //transform.setIdentity();

        //l_qPitch.setEuler( -(m_pRotate->getZ() * SIMD_RADS_PER_DEG) ,
        //                 -(m_pRotate->getX() * SIMD_RADS_PER_DEG) ,
        //                 -(m_pRotate->getY() * SIMD_RADS_PER_DEG));
        //
        //btVector3 l_tempz1(-m_pPosition->getX() , -m_pPosition->getZ(), m_pPosition->getY() );
        //transform.setOrigin(l_tempz1);
        //transform.setRotation(l_qPitch);

        //transform.getOpenGLMatrix(m_matrix);

        //glMultMatrixf(m_matrix);

        gluLookAt ( position.x(), position.y(), position.z(),
                    direction.x(), direction.y(), direction.z(),
                    rotation.x(), rotation.y(), rotation.z() );
    }
}

void Camera::init ( void ) {

}

void Camera::update ( DataMsg *dataMsg ) {
//FIXME
//     if ( _transport->code==DO_DRAW_3D ) {
//
//     } else if ( _transport->code==DO_INIT ) {
//         init();
//     }
    Node::update ( dataMsg );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
