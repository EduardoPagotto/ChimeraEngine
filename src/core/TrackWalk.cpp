#include "chimera/core/TrackWalk.hpp"

namespace Chimera {

TrackWalk::TrackWalk() { pVp = nullptr; }

TrackWalk::~TrackWalk() {}

void TrackWalk::init(ViewPoint* _vp) {
    pVp = _vp;
    updateVectors();
}

void TrackWalk::tracking(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        pVp->position += pVp->front * velocity;
    if (direction == BACKWARD)
        pVp->position -= pVp->front * velocity;
    if (direction == LEFT)
        pVp->position -= right * velocity;
    if (direction == RIGHT)
        pVp->position += right * velocity;
}

void TrackWalk::updateVectors() {
    // Normalize the vectors, because their length gets closer to 0 the
    // more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(pVp->front, pVp->worldUp));
    pVp->up = glm::normalize(glm::cross(right, pVp->front));
}

// void TrackWalk::reset() {
//     along = glm::vec3(1.0f, 0.0f, 0.0f);
//     up = glm::vec3(0.0f, 1.0f, 0.0f);
//     forward = glm::vec3(0.0f, 0.0f, -1.0f);
//     walking();
// }

// void TrackWalk::walking() {
//     // TODO: TESTAR e verificar ser bullet ja nao tem este algoritmo !!!

//     float x = glm::dot(along, pVp->position);   //  along * (*m_pPosition); //Dot Product (produto
//                                                 //  escalar) // DotProduct(along, Position); //GLfloat
//                                                 //  x = DotProduct(along, Position);
//     float y = glm::dot(up, pVp->position);      // up * (*m_pPosition); //  DotProduct(up, Position);
//     float z = glm::dot(forward, pVp->position); // forward * (*m_pPosition); // DotProduct(forward, Position);

//     float ViewMatrix[4][4];

//     ViewMatrix[0][0] = along.x;
//     ViewMatrix[0][1] = up.x;
//     ViewMatrix[0][2] = -forward.x;
//     ViewMatrix[0][3] = 0.0;

//     ViewMatrix[1][0] = along.y;
//     ViewMatrix[1][1] = up.y;
//     ViewMatrix[1][2] = -forward.y;
//     ViewMatrix[1][3] = 0.0;

//     ViewMatrix[2][0] = along.z;
//     ViewMatrix[2][1] = up.z;
//     ViewMatrix[2][2] = -forward.z;
//     ViewMatrix[2][3] = 0.0;

//     ViewMatrix[3][0] = -x;
//     ViewMatrix[3][1] = -y;
//     ViewMatrix[3][2] = z;
//     ViewMatrix[3][3] = 1.0;

//     // FIXME: modelar direito usando as matrizes do glm
//     // glMatrixMode(GL_MODELVIEW);
//     // glLoadMatrixf((GLfloat*)&ViewMatrix);
// }

// void TrackWalk::yaw(float _theta) {

//     // along = along * cos(_theta * DEG2RAD) + forward * sin(_theta * (DEG2RAD);
//     // // along = along * cos(_theta * SIMD_RADS_PER_DEG) + forward * sin(_theta * SIMD_RADS_PER_DEG);
//     // along = glm::normalize(along);
//     // forward = glm::cross(along, up) * -1.0f; // (along % up) * -1.0f;
//     // walking();
// }

// void TrackWalk::pitch(float _theta) {
//     // // Invert UP/DOWN for air cameras
//     // if (type == WalkType::Air) {
//     //     _theta = -_theta;
//     // }

//     // forward = forward * cos(_theta * DEG2RAD) + up * sin(_theta * DEG2RAD);
//     // // forward = forward * cos(_theta * SIMD_RADS_PER_DEG) + up * sin(_theta * SIMD_RADS_PER_DEG);
//     // forward = glm::normalize(forward);

//     // up = glm::cross(forward, along) * -1.0f;
//     // //(forward % along) * -1.0f;
//     // // CrossProduct(forward, along) * -1.0;
//     // walking();
// }

// void TrackWalk::roll(float _theta) {
//     // if (type == WalkType::Land) {
//     //     return; // Not for land cams
//     // }

//     // up = up * cos(_theta * DEG2RAD) - along * sin(_theta * DEG2RAD);
//     // // up = up * cos(_theta * SIMD_RADS_PER_DEG) - along * sin(_theta * SIMD_RADS_PER_DEG);
//     // up = glm::normalize(up);
//     // along = glm::cross(forward, up); // ( forward% up ); //CrossProduct(forward, up); walking();
// }

// void TrackWalk::walk(float _delta, bool _wall[4]) {

//     // // int mult_a = 0;
//     // // int mult_b = 0;
//     // // if (!(_wall[0] && forward.x() * _delta > 0.0 || _wall[1] && forward.x() * _delta < 0.0))
//     // //     mult_a = 1;

//     // // if (!(_wall[2] && forward.z() * _delta > 0.0 || _wall[3] && forward.z() * _delta < 0.0))
//     // //     mult_b = 1;

//     // // if (type == WalkType::Land)
//     // //     pVp->position -= glm::vec3(forward.x() * mult_a, 0.0, forward.z() * mult_b) * delta;
//     // // else
//     // pVp->position -= forward * _delta; // Air camera

//     // walking();
// }

// void TrackWalk::strafe(float _delta, bool _wall[4]) {

//     // int mult_a = 0;
//     // int mult_b = 0;
//     // if (!(_wall[0] && (along.x() * _delta > 0.0) || (_wall[1] && (along.x() * _delta < 0.0))))
//     //     mult_a = 1;

//     // if (!(_wall[2] && along.z() * _delta > 0.0 || _wall[3] && along.z() * _delta < 0.0))
//     //     mult_b = 1;

//     // if (type == WalkType::Land)
//     //     pVp->position -= glm::vec3(along.x() * mult_a, 0.0, along.z() * multi_b) * _delta;
//     // else
//     pVp->position += along * _delta; // Air camera

//     walking();
// }

// void TrackWalk::fly(float _delta, bool _wall[4]) {

//     // Don't allow for land cameras
//     if (type == WalkType::Land) {
//         return;
//     }

//     pVp->position += up * _delta;

//     walking();
// }
} // namespace Chimera
