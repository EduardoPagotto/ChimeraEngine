#include "TrackCam.h"

namespace Chimera {

TrackCam::TrackCam ( std::string id, std::string name ) : Camera ( CameraType::AIR_CAM,id,name ) {

    horizontal = 0.0f;
    vertical = 0.0f;
    distancia = 0;
    distanciaMin = 1.0f;
    distanciaMax = 500.0f;

    along.setValue ( 1.0f, 0.0f, 0.0f );
    up.setValue ( 0.0f, 1.0f, 0.0f );
    forward.setValue ( 0.0f,  0.0f, -1.0f );

}

TrackCam::TrackCam ( const TrackCam& trackCam ) : Camera ( trackCam ) {
    
    horizontal = trackCam.horizontal;
    vertical = trackCam.vertical;
    distancia = trackCam.distancia;
    distanciaMin = trackCam.distanciaMin;
    distanciaMax = trackCam.distanciaMax;
    
    along = trackCam.along;
    up = trackCam.up;
    forward = trackCam.forward;
       
}

TrackCam::~TrackCam() {
}

void TrackCam::update ( DataMsg *dataMsg ) {
    Camera::update ( dataMsg );
}

void TrackCam::init ( void ) {
    
    Camera::init();
    
    reset();
    
    Pitch ( -rotation.x() );
    Roll ( rotation.y() );
    Yaw ( rotation.z() );
    
    initTrackBall();
}

void TrackCam::reset() {
    along.setValue ( 1.0f, 0.0f, 0.0f );
    up.setValue ( 0.0f, 1.0f, 0.0f );
    forward.setValue ( 0.0f,  0.0f, -1.0f );

    updateTrack();
}

void TrackCam::initTrackBall ( void ) {
    distancia = position.distance ( direction );
    //Ajuste inicial da posicao da camera em relacao as coordenada do arquivo de posicao
    vertical = asin ( ( btFabs ( position.z() ) - btFabs ( direction.z() ) ) / distancia ) / 0.017453293f;
    horizontal = asin ( ( btFabs ( position.y() ) - btFabs ( direction.y() ) ) / distancia ) / 0.017453293f;
}



void TrackCam::updateTrack() {
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

void TrackCam::Yaw ( GLfloat theta ) {
    along = along * cos ( theta * SIMD_RADS_PER_DEG ) + forward * sin ( theta * SIMD_RADS_PER_DEG ); //along = along * cos(theta * DEG2RAD) + forward * sin(theta * DEG2RAD);
    along.normalize();

    forward = along.cross ( up ) * -1.0f; // (along % up) * -1.0f;
    updateTrack();
}

void TrackCam::Pitch ( GLfloat theta ) {
    // Invert UP/DOWN for air cameras
    if ( cameraType == CameraType::AIR_CAM )
        theta = -theta;

    forward = forward * cos ( theta * SIMD_RADS_PER_DEG ) + up * sin ( theta * SIMD_RADS_PER_DEG );
    forward.normalize();
    up = forward.cross ( along ) * -1.0f; //(forward % along) * -1.0f;//CrossProduct(forward, along) * -1.0;
    updateTrack();
}

void TrackCam::Roll ( GLfloat theta ) {
    if ( cameraType == CameraType::LAND_CAM )
        return; // Not for land cams

    up = up * cos ( theta * SIMD_RADS_PER_DEG ) - along * sin ( theta * SIMD_RADS_PER_DEG );
    up.normalize();
    along = forward.cross ( up ); // ( forward % up ); //CrossProduct(forward, up);
    updateTrack();
}

void TrackCam::Walk ( GLfloat delta, bool Wall[4] ) {
    if ( cameraType == CameraType::LAND_CAM )
        position -= btVector3 ( forward.x() * ! ( Wall[0] && forward.x() * delta > 0.0 || Wall[1] && forward.x() * delta < 0.0 ),
                                0.0,
                                forward.z() * ! ( Wall[2] && forward.z() * delta > 0.0 ||  Wall[3] && forward.z() * delta < 0.0 ) ) * delta;
    else
        position -= forward * delta; // Air camera

    updateTrack();
}

void TrackCam::Strafe ( GLfloat delta, bool Wall[4] ) {
    if ( cameraType == CameraType::LAND_CAM )
        position -= btVector3 ( along.x() * ! ( Wall[0] && along.x() * delta > 0.0 || Wall[1] && along.x() * delta < 0.0 ),
                                0.0,
                                along.z() * ! ( Wall[2] && along.z() * delta > 0.0 || Wall[3] && along.z() * delta < 0.0 ) ) * delta;
    else
        position += along * delta; // Air camera

    updateTrack();
}

void TrackCam::Fly ( GLfloat delta, bool Wall[4] ) {
    // Don't allow for land cameras
    if ( cameraType == CameraType::LAND_CAM )
        return;

    position += up * delta;
    updateTrack();
}

void TrackCam::trackBall ( int _mx, int _my, int _mz ) {
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

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
