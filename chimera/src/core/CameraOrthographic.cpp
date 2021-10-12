#include "chimera/core/CameraOrthographic.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CameraOrthographic::CameraOrthographic(const float& left, const float& right, const float& botton, const float& top)
    : projectionMatrix(glm::ortho(left, right, botton, top, -1.0f, 1.0f)), viewMatrix(glm::mat4(1.0f)) {

    rotation = 0.0f;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void CameraOrthographic::recalculateMatrix() {
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    viewMatrix = glm::inverse(transform);
    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

} // namespace Chimera