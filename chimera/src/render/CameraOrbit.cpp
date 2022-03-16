#include "chimera/render/CameraOrbit.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CameraOrbit::CameraOrbit(const glm::vec3& pos, const glm::vec3& up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, 0.0f)), fov(CAMERA_MAX_FOV) {
    this->position = pos;
    this->up = up;
    this->yaw = yaw;
    this->pitch = pitch;

    this->nearPlane = 0.3f;
    this->farPlane = 5000.0f;

    min = 1.0f;
    max = 1500.0f;

    distance = glm::distance(this->position, this->front);
    // this->yaw = asin((glm::abs(this->position.z) - glm::abs(this->front.z)) / distance) / 0.017453293f;
    // this->pitch = asin((glm::abs(this->position.y) - glm::abs(this->front.y)) / distance) / 0.017453293f;

    updateVectors();
}

CameraOrbit::~CameraOrbit() {}

void CameraOrbit::setViewportSize(const uint32_t& width, const uint32_t& height) {
    float aspectRatio = (float)width / (float)height;
    // TODO: criar funcao de recalc de projection para usar a variacao de FOV
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void CameraOrbit::updateEye() {
    if (eye.size() == 1) {
        eye.update(glm::lookAt(position, front, up), projectionMatrix);
    } else {
        glm::vec3 novaPositionL, novaFrontL, novaPositionR, novaFrontR;
        glm::vec3 left_p = front - position; // front and position as points
        glm::vec3 cross1 = glm::cross(up, left_p);
        glm::vec3 norm1 = glm::normalize(cross1);
        glm::vec3 final_norm1 = norm1 * eye.getNoseDist();

        novaPositionL = position + final_norm1;
        novaFrontL = front + final_norm1;
        eye.getHead()[0].update(glm::lookAt(novaPositionL, novaFrontL, up), projectionMatrix); // Left

        novaPositionR = position - final_norm1;
        novaFrontR = front - final_norm1;
        eye.getHead()[1].update(glm::lookAt(novaPositionR, novaFrontR, up), projectionMatrix); // Right
    }
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

    updateVectors();

    this->updateEye();
}

void CameraOrbit::processDistance(const int& _mz) {
    distance += _mz;
    if (distance < min)
        distance = min;
    if (distance > max)
        distance = max;
}

void CameraOrbit::processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch) {

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