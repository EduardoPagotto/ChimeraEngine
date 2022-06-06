#pragma once
#include "ICamera.hpp"

namespace Chimera {

class CameraOrthographic : public ICamera {

  public:
    CameraOrthographic(const float& xmag, const float& ymag, const float& near, const float& far);
    virtual ~CameraOrthographic();

    void setRotation(float rot) { rotation = rot; }
    const float getRotation() const { return rotation; }
    virtual void setPosition(const glm::vec3& position) override { this->position = position; };
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::mat4& getProjection() const override { return projection->getProjection(); }
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override;

  private:
    Orthogonal* projection;
    float rotation;
    glm::vec3 position;
};
} // namespace Chimera
