#include "Camera.h"

namespace Chimera {

Camera::Camera () : Node ( EntityType::CAMERA ) {
    
    type = CameraType::AIR_CAM;
    near = 0.1f;
    far = 1000.0f;
    fov = 45.0f;
 
}

Camera::Camera (const Camera& _camera ) : Node ( _camera ) {
    
    type = _camera.type;
    near = _camera.near;
    far = _camera.far;
    fov = _camera.fov;
    
}

Camera::~Camera() {
}

void Camera::exec ( void ) {
    if ( getStatus() ==true ) {
        //btScalar m_matrix[16];

        //btTransform transform;
        //btQuaternion l_qpitch;

        //transform.setIdentity();

        //l_qpitch.setEuler( -(m_pRotate->getZ() * SIMD_RADS_PER_DEG) ,
        //                 -(m_pRotate->getX() * SIMD_RADS_PER_DEG) ,
        //                 -(m_pRotate->getY() * SIMD_RADS_PER_DEG));
        //
        //btVector3 l_tempz1(-m_pPosition->getX() , -m_pPosition->getZ(), m_pPosition->getY() );
        //transform.setOrigin(l_tempz1);
        //transform.setRotation(l_qpitch);

        //transform.getOpenGLMatrix(m_matrix);

        //glMultMatrixf(m_matrix);

//         Transform *trans = (Transform*)parent;        
//         gluLookAt ( trans->getPosition().x(), trans->getPosition().y(), trans->getPosition().z(),
//                     trans->getDirection().x(), trans->getDirection().y(), trans->getDirection().z(),
//                     trans->getRotation().x(), trans->getRotation().y(), trans->getRotation().z() );
    }
}

void Camera::init ( void ) {

}

void Camera::update ( DataMsg *_dataMsg ) {
//FIXME
//     if ( _transport->code==DO_DRAW_3D ) {
//
//     } else if ( _transport->code==DO_INIT ) {
//         init();
//     }
    Node::update ( _dataMsg );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
