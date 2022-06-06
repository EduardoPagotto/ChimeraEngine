#include "chimera/core/visible/CameraOrthographic.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CameraOrthographic::CameraOrthographic(const float& xmag, const float& ymag, const float& near, const float& far)
    : rotation(0.0f), position(glm::vec3(0.0f)) {
    projection = new Orthogonal(xmag, ymag, near, far);
}

CameraOrthographic::~CameraOrthographic() { delete projection; }

// void CameraOrthographic::updateEye(EyeView& eye) { //FIXME: incorporar!!!!
//     glm::mat4 transform =
//         glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
//     eye.update(glm::inverse(transform), projectionMatrix);
// }

void CameraOrthographic::setViewportSize(const uint32_t& width, const uint32_t& height) {
    projection->setViewportProjection(width, height);
}
} // namespace Chimera