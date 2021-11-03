#ifndef __CHIMERA_CAMERA_ORTHOGRAPHIC_HPP
#define __CHIMERA_CAMERA_ORTHOGRAPHIC_HPP

#include "ICamera.hpp"

namespace Chimera {

class CameraOrthographic : public ICamera {

  public:
    CameraOrthographic(const float& size, const float& nearClip, const float& farClip);
    virtual ~CameraOrthographic() {}

    void setRotation(float rot) {
        rotation = rot;
        recalculateMatrix(false);
    }
    const float getRotation() const { return rotation; }

    virtual void setPosition(const glm::vec3& position) override;
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::mat4 getViewMatrix() const override { return viewMatrix; };
    virtual const glm::mat4 getProjectionMatrix() const override { return projectionMatrix; }
    virtual const glm::mat4 getViewProjectionMatrix() const override { return viewProjectionMatrix; }
    virtual const glm::mat4 getViewProjectionMatrixInverse() const override { return viewProjectionMatrixInverse; };
    virtual const bool is3D() const override { return false; }
    virtual const glm::mat4 recalculateMatrix(bool left) override;
    virtual void processInput(float deltaTime) override;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override;

  private:
    float rotation, aspectRatio, size, nearClip, farClip;
    glm::vec3 position;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 viewProjectionMatrixInverse;
};
} // namespace Chimera
#endif