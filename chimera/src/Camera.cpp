#include "Camera.h"

namespace Chimera {

Camera::Camera ( std::string name, CameraType _camType ) : Transform ( EntityType::CAMERA, name ), cameraType ( _camType ) {

    setViewport ( Rectangular ( 0, 0, 640, 480 ) );
    setProjectionPerspective ( 45.0f,  this->aspectRatio() ,  0.1f, far );

    horizontal = 0.0f;
    vertical = 0.0f;
    distancia = 0;
    distanciaMin = 1.0f;
    distanciaMax = 500.0f;

    target.setZero();

    along.setValue ( 1.0f, 0.0f, 0.0f );
    up.setValue ( 0.0f, 1.0f, 0.0f );
    forward.setValue ( 0.0f,  0.0f, -1.0f );

}

Camera::~Camera() {
}

void Camera::reset() {
    along.setValue ( 1.0f, 0.0f, 0.0f );
    up.setValue ( 0.0f, 1.0f, 0.0f );
    forward.setValue ( 0.0f,  0.0f, -1.0f );

    update();
}

void Camera::update() {
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

void Camera::Yaw ( GLfloat theta ) {
    along = along * cos ( theta * SIMD_RADS_PER_DEG ) + forward * sin ( theta * SIMD_RADS_PER_DEG ); //along = along * cos(theta * DEG2RAD) + forward * sin(theta * DEG2RAD);
    along.normalize();

    forward = along.cross ( up ) * -1.0f; // (along % up) * -1.0f;
    update();
}

void Camera::Pitch ( GLfloat theta ) {
    // Invert UP/DOWN for air cameras
    if ( cameraType == CameraType::AIR_CAM )
        theta = -theta;

    forward = forward * cos ( theta * SIMD_RADS_PER_DEG ) + up * sin ( theta * SIMD_RADS_PER_DEG );
    forward.normalize();
    up = forward.cross ( along ) * -1.0f; //(forward % along) * -1.0f;//CrossProduct(forward, along) * -1.0;
    update();
}

void Camera::Roll ( GLfloat theta ) {
    if ( cameraType == CameraType::LAND_CAM )
        return; // Not for land cams

    up = up * cos ( theta * SIMD_RADS_PER_DEG ) - along * sin ( theta * SIMD_RADS_PER_DEG );
    up.normalize();
    along = forward.cross ( up ); // ( forward % up ); //CrossProduct(forward, up);
    update();
}

void Camera::Walk ( GLfloat delta, bool Wall[4] ) {
    if ( cameraType == CameraType::LAND_CAM )
        position -= btVector3 ( forward.x() * ! ( Wall[0] && forward.x() * delta > 0.0 || Wall[1] && forward.x() * delta < 0.0 ),
                                0.0,
                                forward.z() * ! ( Wall[2] && forward.z() * delta > 0.0 ||  Wall[3] && forward.z() * delta < 0.0 ) ) * delta;
    else
        position -= forward * delta; // Air camera

    update();
}

void Camera::Strafe ( GLfloat delta, bool Wall[4] ) {
    if ( cameraType == CameraType::LAND_CAM )
        position -= btVector3 ( along.x() * ! ( Wall[0] && along.x() * delta > 0.0 || Wall[1] && along.x() * delta < 0.0 ),
                                0.0,
                                along.z() * ! ( Wall[2] && along.z() * delta > 0.0 || Wall[3] && along.z() * delta < 0.0 ) ) * delta;
    else
        position += along * delta; // Air camera

    update();
}

void Camera::Fly ( GLfloat delta, bool Wall[4] ) {
    // Don't allow for land cameras
    if ( cameraType == CameraType::LAND_CAM )
        return;

    position += up * delta;
    update();
}

//--init new-- 20110617
void Camera::setProjectionPerspective ( float _fov, float _ratio, float _near, float _far ) {
    perspective = true;
    fov = _fov;
    ratio = _ratio;
    near = _near;
    far = _far;
}

void Camera::setProjectionOrtogonal() {
    perspective = false;
}

void Camera::setViewport ( const Rectangular _tela ) {
    tela = _tela;
    ratio = tela.calcRatio();
}

void Camera::projection ( void ) {
    glViewport ( tela.initX , tela.initY , tela.width, tela.height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    if ( perspective==true )
        gluPerspective ( fov, ( GLfloat ) aspectRatio(), near, far );
    else
        glOrtho ( tela.initX, tela.width, tela.initY, tela.height,-1.0,1.0 );

    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
}

void Camera::trackBall ( int _mx, int _my, int _mz ) {
    if ( getMode() ==0 ) {
        horizontal += _mx;
        vertical += _my;
        if ( _mz ) {
            float l_distTemp = distancia; //tempDistancia = distancia;
            l_distTemp += _mz;
            if ( ( l_distTemp > distanciaMin ) && ( l_distTemp < distanciaMax ) )
                distancia = l_distTemp;
        }

        float l_kx = horizontal * 0.017453293f;
        float l_ky = vertical * 0.017453293f;

        position.setY ( distancia * cos ( l_kx ) * cos ( l_ky ) );
        position.setZ ( distancia * sin ( l_kx ) );
        position.setX ( distancia * cos ( l_kx ) * sin ( l_ky ) );
    }
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
                    target.x(), target.y(), target.z(),
                    0,0,1 );
    }
}

void Camera::initTrackBall ( void ) {
    distancia = position.distance ( target );
    //Ajuste inicial da posicao da camera em relacao as coordenada do arquivo de posicao
    vertical = asin ( ( btFabs ( position.z() ) - btFabs ( target.z() ) ) / distancia ) / 0.017453293f;
    horizontal = asin ( ( btFabs ( position.y() ) - btFabs ( target.y() ) ) / distancia ) / 0.017453293f;
}

void Camera::init ( void ) {
    reset();

    Pitch ( -rotation.x() );
    Roll ( rotation.y() );
    Yaw ( rotation.z() );

    initTrackBall();
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
