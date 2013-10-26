#include "CameraMan.h"

namespace Chimera {

CameraMan::CameraMan (std::string _id, std::string _name ) : Camera (CameraType::Spherical,_id,_name) {

    along.setValue ( 1.0f, 0.0f, 0.0f );
    up.setValue ( 0.0f, 1.0f, 0.0f );
    forward.setValue ( 0.0f,  0.0f, -1.0f );

}

CameraMan::CameraMan ( const CameraMan& _cameraMan ) : Camera ( _cameraMan ) {

    along = _cameraMan.along;
    up = _cameraMan.up;
    forward = _cameraMan.forward;

}

CameraMan::CameraMan ( const Camera& _camera ) : Camera ( _camera ) {

    type = CameraType::Spherical;

    along.setValue ( 1.0f, 0.0f, 0.0f );
    up.setValue ( 0.0f, 1.0f, 0.0f );
    forward.setValue ( 0.0f,  0.0f, -1.0f );

}

CameraMan::~CameraMan() {
}

void CameraMan::clone(Node **ppNode ) {
    *ppNode = new CameraMan( *this ); 
    Node::clone( ppNode );  
}

void CameraMan::update ( DataMsg *_dataMsg ) {
    Camera::update ( _dataMsg );
    if ( _dataMsg->getKindOp() ==KindOp::START ) {
        init();
        reset();
        pitch ( -rotation.x() );
        roll ( rotation.y() );
        yaw ( rotation.z() );
    }
}

void CameraMan::reset() {

    along.setValue ( 1.0f, 0.0f, 0.0f );
    up.setValue ( 0.0f, 1.0f, 0.0f );
    forward.setValue ( 0.0f,  0.0f, -1.0f );

    updateMove();
}

void CameraMan::updateMove() {
    //TODO: TESTAR e verificar ser bullet ja nao tem este algoritmo !!!

    GLfloat x = along.dot ( position ); //  along * (*m_pPosition); //Dot Product (produto escalar) // DotProduct(along, Position); //GLfloat x = DotProduct(along, Position);
    GLfloat y = up.dot ( position );    //up * (*m_pPosition); //  DotProduct(up, Position);
    GLfloat z = forward.dot ( position ); // forward * (*m_pPosition); // DotProduct(forward, Position);

    GLfloat ViewMatrix[4][4];

    ViewMatrix[0][0] = along.x();
    ViewMatrix[0][1] = up.x();
    ViewMatrix[0][2] = -forward.x();
    ViewMatrix[0][3] = 0.0;

    ViewMatrix[1][0] = along.y();
    ViewMatrix[1][1] = up.y();
    ViewMatrix[1][2] = -forward.y();
    ViewMatrix[1][3] = 0.0;

    ViewMatrix[2][0] = along.z();
    ViewMatrix[2][1] = up.z();
    ViewMatrix[2][2] = -forward.z();
    ViewMatrix[2][3] = 0.0;

    ViewMatrix[3][0] = -x;
    ViewMatrix[3][1] = -y;
    ViewMatrix[3][2] = z;
    ViewMatrix[3][3] = 1.0;

    glMatrixMode ( GL_MODELVIEW );
    glLoadMatrixf ( ( GLfloat * ) &ViewMatrix );
}

void CameraMan::yaw ( GLfloat _theta ) {
    along = along * cos ( _theta * SIMD_RADS_PER_DEG ) + forward * sin ( _theta * SIMD_RADS_PER_DEG ); //along = along * cos(theta * DEG2RAD) + forward * sin(theta * DEG2RAD);
    along.normalize();

    forward = along.cross ( up ) * -1.0f; // (along % up) * -1.0f;
    updateMove();
}

void CameraMan::pitch ( GLfloat _theta ) {
    // Invert UP/DOWN for air cameras
    if ( type == CameraType::Air )
        _theta = -_theta;

    forward = forward * cos ( _theta * SIMD_RADS_PER_DEG ) + up * sin ( _theta * SIMD_RADS_PER_DEG );
    forward.normalize();
    up = forward.cross ( along ) * -1.0f; //(forward % along) * -1.0f;//CrossProduct(forward, along) * -1.0;
    updateMove();
}

void CameraMan::roll ( GLfloat _theta ) {
    if ( type == CameraType::Land )
        return; // Not for land cams

    up = up * cos ( _theta * SIMD_RADS_PER_DEG ) - along * sin ( _theta * SIMD_RADS_PER_DEG );
    up.normalize();
    along = forward.cross ( up ); // ( forward % up ); //CrossProduct(forward, up);
    updateMove();
}

void CameraMan::walk ( GLfloat _delta, bool _wall[4] ) {

    if ( type == CameraType::Land )
        position -= btVector3 ( forward.x() * ! ( _wall[0] && forward.x() * _delta > 0.0 || _wall[1] && forward.x() * _delta < 0.0 ),
                                0.0,
                                forward.z() * ! ( _wall[2] && forward.z() * _delta > 0.0 ||  _wall[3] && forward.z() * _delta < 0.0 ) ) * _delta;
    else
        position -= forward * _delta; // Air camera

    updateMove();
}

void CameraMan::strafe ( GLfloat _delta, bool _wall[4] ) {

    if ( type == CameraType::Land )
        position -= btVector3 ( along.x() * ! ( _wall[0] && along.x() * _delta > 0.0 || _wall[1] && along.x() * _delta < 0.0 ),
                                0.0,
                                along.z() * ! ( _wall[2] && along.z() * _delta > 0.0 || _wall[3] && along.z() * _delta < 0.0 ) ) * _delta;
    else
        position += along * _delta; // Air camera

    updateMove();
}

void CameraMan::fly ( GLfloat _delta, bool _wall[4] ) {

    // Don't allow for land cameras
    if ( type == CameraType::Land )
        return;

    position += up * _delta;

    updateMove();
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
