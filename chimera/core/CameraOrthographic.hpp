#ifndef __CHIMERA_CAMERA_ORTHOGRAPHIC_HPP
#define __CHIMERA_CAMERA_ORTHOGRAPHIC_HPP

#include <glm/glm.hpp>

namespace Chimera {

class CameraOrthographic {

  public:
    CameraOrthographic(const float& left, const float& right, const float& botton, const float& top);
    virtual ~CameraOrthographic() {}

    void setPosition(const glm::vec3& position) {
        this->position = position;
        recalculateMatrix();
    }
    const glm::vec3 getPosition() const { return position; }

    void setRotation(float rot) {
        rotation = rot;
        recalculateMatrix();
    }
    const float getRotation() const { return rotation; }
    const glm::mat4 getViewMatrix() const { return viewMatrix; };
    const glm::mat4 getProjectionMatrix() const { return projectionMatrix; }
    const glm::mat4 getViewProjectionMatrix() const { return viewProjectionMatrix; }

  private:
    void recalculateMatrix();
    float rotation;
    glm::vec3 position;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;
};
} // namespace Chimera
#endif