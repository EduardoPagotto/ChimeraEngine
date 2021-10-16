#ifndef __CHIMERA_CAMERA_CONTROLER_HPP
#define __CHIMERA_CAMERA_CONTROLER_HPP

#include <glm/glm.hpp>

namespace Chimera {

class Camera {
  public:
    Camera() = default;
    virtual ~Camera() = default;

    const float getAspectRatio() const { return aspectRatio; }
    const float getNear() const { return nearPlane; }
    const float getFar() const { return farPlane; }

    const glm::vec3 getPosition() const { return position; }

    const glm::mat4 getViewMatrix() const { return viewMatrix; }
    const glm::mat4 getProjectionMatrix() const { return projectionMatrix; }
    const glm::mat4 getViewProjectionMatrix() const { return viewProjectionMatrix; }
    const glm::mat4 getViewProjectionMatrixInverse() const { return viewProjectionMatrixInverse; }

    void setViewportSize(const uint32_t& width, const uint32_t& height) { aspectRatio = (float)width / (float)height; }

    void setNear(const float& near) { this->nearPlane = near; }
    void setFar(const float& far) { this->farPlane = far; }
    void setPosition(const glm::vec3& position) { this->position = position; }
    void incrementPosition(const glm::vec3& inc) { this->position += inc; }

    void updateAllMatrix(const glm::mat4& projection, const glm::mat4& view) {

        this->viewMatrix = view;
        this->projectionMatrix = projection;

        this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;

        glm::mat4 projectionMatrixInverse = glm::inverse(projectionMatrix);
        glm::mat4 viewMatrixInverse = glm::inverse(viewMatrix);

        this->viewProjectionMatrixInverse = viewMatrixInverse * projectionMatrixInverse;
    }

  protected:
    float nearPlane = 0.1, farPlane = 2000.0f, aspectRatio = 1.0f;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 viewProjectionMatrixInverse = glm::mat4(1.0f);
};
} // namespace Chimera
#endif