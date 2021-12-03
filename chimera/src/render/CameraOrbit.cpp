#include "chimera/render/CameraOrbit.hpp"
#include "chimera/core/Keyboard.hpp"
#include "chimera/core/MouseDevice.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CameraOrbit::CameraOrbit(const glm::vec3& pos, const glm::vec3& up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, 0.0f)), fov(CAMERA_MAX_FOV), eyeIndex(0) {
    this->position = pos;
    this->up = up;
    this->yaw = yaw;
    this->pitch = pitch;

    this->nearPlane = 0.3f;
    this->farPlane = 5000.0f;

    this->aspectRatio = 1.0f;

    min = 1.0f;
    max = 1500.0f;

    distance = glm::distance(this->position, this->front);
    // this->yaw = asin((glm::abs(this->position.z) - glm::abs(this->front.z)) / distance) / 0.017453293f;
    // this->pitch = asin((glm::abs(this->position.y) - glm::abs(this->front.y)) / distance) / 0.017453293f;

    updateVectors();
}

CameraOrbit::~CameraOrbit() {}

void CameraOrbit::setViewportSize(const uint32_t& width, const uint32_t& height) {
    aspectRatio = (float)width / (float)height;
    // TODO: criar funcao de recalc de projection para usar a variacao de FOV
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

const glm::mat4 CameraOrbit::recalculateMatrix(const uint8_t& eyeIndex) {
    this->eyeIndex = eyeIndex;
    if (eyeIndex == 0) {
        eyeMat[eyeIndex].viewMatrix = glm::lookAt(position, front, up);
    } else {

        float distEye = 5.0; // right
        if (eyeIndex == 1)   // Left
            distEye = -5.0;

        glm::vec3 left_p = front - position; // front and position as points
        glm::vec3 cross1 = glm::cross(up, left_p);
        glm::vec3 norm1 = glm::normalize(cross1);
        glm::vec3 final_norm1 = norm1 * distEye;

        glm::vec3 novaPosition = position - final_norm1;
        glm::vec3 novaFront = front - final_norm1;
        eyeMat[eyeIndex].viewMatrix = glm::lookAt(novaPosition, novaFront, up);
    }
    // projectionMatrix so e calculado no dimencionamento do viewport ou alteracao do FOV
    eyeMat[eyeIndex].viewProjectionMatrix = projectionMatrix * eyeMat[eyeIndex].viewMatrix;

    glm::mat4 projectionMatrixInverse = glm::inverse(projectionMatrix);
    glm::mat4 viewMatrixInverse = glm::inverse(eyeMat[eyeIndex].viewMatrix);
    eyeMat[eyeIndex].viewProjectionMatrixInverse = viewMatrixInverse * projectionMatrixInverse;

    return eyeMat[eyeIndex].viewProjectionMatrixInverse;
}

void CameraOrbit::invertPitch() {
    pitch = -pitch;
    updateVectors();
}

void CameraOrbit::updateVectors() {

    float theta = glm::radians(yaw); // yaw * 0.017453293f; ( yaw * (PI/180) )
    float phi = glm::radians(pitch); // pitch * 0.017453293f;
    if (this->up.y == 1) {
        this->position.x = distance * sin(phi) * sin(theta);
        this->position.y = distance * cos(phi);
        this->position.z = distance * sin(phi) * cos(theta);
    } else { // this->up.z == 1 ou -1
        this->position.x = distance * cos(theta) * sin(phi);
        this->position.y = distance * cos(theta) * cos(phi);
        this->position.z = distance * sin(theta);
    }
}

void CameraOrbit::onUpdate(const double& ts) {

    if (MouseDevice::getButtonState(1) == SDL_PRESSED) {
        glm::ivec2 mouseMove = MouseDevice::getMoveRel();
        processCameraRotation(mouseMove.x, mouseMove.y);

    } else if (MouseDevice::getButtonState(3) == SDL_PRESSED) {
        glm::ivec2 mouseMove = MouseDevice::getMoveRel();
        processDistance(mouseMove.y);
    }
}

void CameraOrbit::processDistance(const int& _mz) {
    distance += _mz;
    if (distance < min)
        distance = min;
    if (distance > max)
        distance = max;

    this->updateVectors();
}

// void CameraOrbit::processCameraMovement(glm::vec3& direction, float deltaTime) {
//     // float velocity = movementSpeed * deltaTime;
//     // position += direction * velocity;
// }

void CameraOrbit::processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch) {
    // Make sure the user isn't interacting with the UI
    // if (!Window::GetHideCursor())
    //    return;
    if (this->up.y == 1) {
        yaw -= (float)xOffset;
        pitch -= (float)yOffset;

        // Constrain the pitch
        if (constrainPitch) {
            if (pitch < 1.0f)
                pitch = 1.0f;
            if (pitch > 179.0f)
                pitch = 179.0f;
        }

    } else { // this->->up.z == 1 ou -1

        yaw += (float)yOffset;
        pitch += (float)xOffset;
        // if (yaw < 1.0f)
        //     yaw = 1.0f;
        // if (yaw > 179.0f)
        //     yaw = 179.0f;
    }

    updateVectors();
}

void CameraOrbit::processCameraFOV(double offset) {

    if (offset != 0.0 && fov >= 1.0 && fov <= CAMERA_MAX_FOV) {
        fov -= (float)offset;
    }
    if (fov < 1.0f) {
        fov = 1.0f;
    } else if (fov > CAMERA_MAX_FOV) {
        fov = CAMERA_MAX_FOV;
    }
}
} // namespace Chimera