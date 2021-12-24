#pragma once
#include "ICamera.hpp"

namespace Chimera {

class CameraOrthographic : public ICamera {

  public:
    CameraOrthographic(const float& size, const float& nearClip, const float& farClip);
    virtual ~CameraOrthographic() {}

    void setRotation(float rot) {
        rotation = rot;
        recalculateMatrix(0);
    }
    const float getRotation() const { return rotation; }

    virtual void setPosition(const glm::vec3& position) override;
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::mat4& getProjectionMatrix() const override { return projectionMatrix; }
    virtual const bool is3D() const override { return false; }
    virtual const glm::mat4& recalculateMatrix(const uint8_t& eyeIndex) override;
    virtual void onUpdate(const double& ts) override;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override;
    virtual EyeView* view() override { return &eye; }

  private:
    float rotation, size, nearClip, farClip;
    glm::vec3 position;
    glm::mat4 projectionMatrix;
    EyeView eye;
};
} // namespace Chimera
