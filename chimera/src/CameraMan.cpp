#include "CameraMan.h"
#include "OpenGLDefs.h"

namespace Chimera {

CameraMan::CameraMan(std::string _name) : Camera(nullptr, CameraType::Spherical, _name) {

    along = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
}

CameraMan::CameraMan(const Camera& _camera) : Camera(_camera) {

    type = CameraType::Spherical;

    along = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
}

CameraMan::~CameraMan() {}

void CameraMan::init() {

    Camera::init();
    reset();
    pitch(-rotation.x);
    roll(rotation.y);
    yaw(rotation.z);
}

void CameraMan::reset() {

    along = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);

    updateMove();
}

void CameraMan::render() { updateMove(); }

void CameraMan::updateMove() {
    // TODO: TESTAR e verificar ser bullet ja nao tem este algoritmo !!!

    GLfloat x =
        glm::dot(along, position); //  along * (*m_pPosition); //Dot Product (produto
                                   //  escalar) // DotProduct(along, Position); //GLfloat
                                   //  x = DotProduct(along, Position);
    GLfloat y =
        glm::dot(up, position); // up * (*m_pPosition); //  DotProduct(up, Position);
    GLfloat z = glm::dot(
        forward, position); // forward * (*m_pPosition); // DotProduct(forward, Position);

    GLfloat ViewMatrix[4][4];

    ViewMatrix[0][0] = along.x;
    ViewMatrix[0][1] = up.x;
    ViewMatrix[0][2] = -forward.x;
    ViewMatrix[0][3] = 0.0;

    ViewMatrix[1][0] = along.y;
    ViewMatrix[1][1] = up.y;
    ViewMatrix[1][2] = -forward.y;
    ViewMatrix[1][3] = 0.0;

    ViewMatrix[2][0] = along.z;
    ViewMatrix[2][1] = up.z;
    ViewMatrix[2][2] = -forward.z;
    ViewMatrix[2][3] = 0.0;

    ViewMatrix[3][0] = -x;
    ViewMatrix[3][1] = -y;
    ViewMatrix[3][2] = z;
    ViewMatrix[3][3] = 1.0;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf((GLfloat*)&ViewMatrix);
}

void CameraMan::yaw(float _theta) {
    //     along = along * cos ( _theta * SIMD_RADS_PER_DEG ) + forward * sin ( _theta *
    //     SIMD_RADS_PER_DEG ); //along = along * cos(theta * DEG2RAD) + forward *
    //     sin(theta * DEG2RAD); along.normalize();
    //
    //     forward = along.cross ( up ) * -1.0f; // (along % up) * -1.0f;
    //     updateMove();
}

void CameraMan::pitch(float _theta) {
    //     // Invert UP/DOWN for air cameras
    //     if ( type == CameraType::Air ) {
    //         _theta = -_theta;
    //     }
    //
    //     forward = forward * cos ( _theta * SIMD_RADS_PER_DEG ) + up * sin ( _theta *
    //     SIMD_RADS_PER_DEG ); forward.normalize(); up = forward.cross ( along ) * -1.0f;
    //     //(forward % along) * -1.0f;//CrossProduct(forward, along) * -1.0;
    //     updateMove();
}

void CameraMan::roll(float _theta) {
    //     if ( type == CameraType::Land ) {
    //         return;    // Not for land cams
    //     }
    //
    //     up = up * cos ( _theta * SIMD_RADS_PER_DEG ) - along * sin ( _theta *
    //     SIMD_RADS_PER_DEG ); up.normalize(); along = forward.cross ( up ); // ( forward
    //     % up ); //CrossProduct(forward, up); updateMove();
}

void CameraMan::walk(float _delta, bool _wall[4]) {

    //     if ( type == CameraType::Land )
    //         position -= glm::vec3 ( forward.x() * ! ( _wall[0] && forward.x() * _delta
    //         > 0.0 || _wall[1] && forward.x() * _delta < 0.0 ),
    //                                 0.0,
    //                                 forward.z() * ! ( _wall[2] && forward.z() * _delta
    //                                 > 0.0 || _wall[3] && forward.z() * _delta < 0.0 ) )
    //                                 * _delta;
    //     else {
    //         position -= forward * _delta;    // Air camera
    //     }
    //
    //     updateMove();
}

void CameraMan::strafe(float _delta, bool _wall[4]) {

    //     if ( type == CameraType::Land )
    //         position -= glm::vec3 ( along.x() * ! ( _wall[0] && along.x() * _delta >
    //         0.0 || _wall[1] && along.x() * _delta < 0.0 ),
    //                                 0.0,
    //                                 along.z() * ! ( _wall[2] && along.z() * _delta >
    //                                 0.0 || _wall[3] && along.z() * _delta < 0.0 ) ) *
    //                                 _delta;
    //     else {
    //         position += along * _delta;    // Air camera
    //     }
    //
    //     updateMove();
}

void CameraMan::fly(float _delta, bool _wall[4]) {

    //     // Don't allow for land cameras
    //     if ( type == CameraType::Land ) {
    //         return;
    //     }
    //
    //     position += up * _delta;
    //
    //     updateMove();
}
} // namespace Chimera
