#include "chimera/render/CameraOrthographic.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CameraOrthographic::CameraOrthographic(const float& size, const float& nearClip, const float& farClip)
    : size(size), nearClip(nearClip), farClip(farClip), rotation(0.0f), position(glm::vec3(0.0f)) {}

const glm::mat4& CameraOrthographic::recalculateMatrix(const uint8_t& eyeIndex) {
    eye.setIndex(eyeIndex);
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    eye.update(glm::inverse(transform), projectionMatrix);

    return eye.getViewProjectionInverse();
}

void CameraOrthographic::setPosition(const glm::vec3& position) {
    this->position = position;
    recalculateMatrix(0);
}

void CameraOrthographic::onUpdate(const double& ts) {}

void CameraOrthographic::setViewportSize(const uint32_t& width, const uint32_t& height) {

    float aspectRatio = (float)width / (float)height;
    float left = -size * aspectRatio * 0.5f;
    float right = size * aspectRatio * 0.5f;
    float bottom = -size * 0.5;
    float top = size * 0.5f;

    projectionMatrix = glm::ortho(left, right, bottom, top, nearClip, farClip);
}
} // namespace Chimera