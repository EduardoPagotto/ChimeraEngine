#include "Camera.h"

namespace Chimera {

Camera::Camera () : Node ( EntityKind::CAMERA ) {
    
    type = CameraType::AIR_CAM;
    near = 0.1f;
    far = 1000.0f;
    fov = 45.0f;
    
    position.setZero();
    rotation.setZero();
    direction.setZero();
    
    perspective = true;
 
}

Camera::Camera (const Camera& _camera ) : Node ( _camera ) {
    
    type = _camera.type;
    near = _camera.near;
    far = _camera.far;
    fov = _camera.fov;
    
    position = _camera.position;
    rotation = _camera.rotation;
    direction = _camera.direction;
    
    perspective = _camera.perspective;
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
       
        gluLookAt ( position.x(), position.y(), position.z(),
                    direction.x(), direction.y(), direction.z(),
                    rotation.x(), rotation.y(), rotation.z() );
    }
}

void Camera::update ( DataMsg *_dataMsg ) {
    
    if  (_dataMsg->getKindOp() == KindOp::START) {
        position = transform.getOrigin();
        
        //FIXME verificar dentro a matrix estes valores
        //position.setValue(-5.0, -5.0, -5.0);
        
        direction.setValue(0.0, 0.0, 0.0);
        rotation.setValue(0.0, 0.0, 1.0);
        
        //direction = transform.getBasis();
       // rotation = transform.getRotation();
     
    } if (_dataMsg->getKindOp() == KindOp::DRAW3D) {
        
    }
    
    Node::update ( _dataMsg );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
