#pragma once
#include "chimera/core/TagComponent.hpp"
#include "chimera/core/visible/EyeView.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

enum class CamKind { FPS = 0, ORBIT = 1, STATIC = 3 };

class ICamera {
  public:
    virtual const glm::mat4& getProjection() const = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;
};

class Camera : public ICamera {
  public:
    Camera(const float& xmag, const float& ymag, const float& near, const float& far)
        : xmag(xmag), ymag(ymag), near(near), far(far), isOrtho(true), fov(0.0f) {}
    Camera(const float& fov, const float& near, const float& far) : fov(fov), near(near), far(far), isOrtho(false) {}
    virtual const glm::mat4& getProjection() const override { return projection; }
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual void setPosition(const glm::vec3& position) override { this->position = position; }
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override {
        if (isOrtho) {
            float halfAspectRatio = (float)width / (float)height * 0.5f;
            float xsize = xmag * halfAspectRatio;
            float ysize = ymag * 0.5f;
            projection = glm::ortho(-xsize, xsize, -ysize, ysize, near, far);
        } else {
            projection = glm::perspective(glm::radians(fov), (float)width / (float)height, near, far);
        }
    }

  private:
    glm::vec3 position = glm::vec3(0, 0, 0);
    float fov = CAMERA_MAX_FOV, xmag = 800.0f, ymag = 600.0f, near = 0.1f, far = 1000.0f;
    bool isOrtho = false;
    glm::mat4 projection = glm::mat4(1.0f);
};

struct CameraComponent {
    TagComponent tag;
    ICamera* camera = nullptr;
    EyeView* eyeView = nullptr;
    bool primary = true;
    bool fixedAspectRatio = false;
    CamKind camKind = CamKind::STATIC;
    float pitch = 0.0f, yaw = 90.0f, min = 0.5f, max = 1000.0f;
    glm::vec3 up = glm::vec3(0, 1, 0);
    CameraComponent() = default;
    CameraComponent(const CameraComponent& o) = default;
};
} // namespace Chimera
