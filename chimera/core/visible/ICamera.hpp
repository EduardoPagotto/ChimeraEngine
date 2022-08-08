#pragma once
#include "chimera/core/TagComponent.hpp"
#include "chimera/core/ViewProjection.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

enum class CamKind { FPS = 0, ORBIT = 1, STATIC = 3 };

class Camera {
  public:
    const glm::mat4& getProjection() const { return projection; }
    const glm::vec3& getPosition() const { return position; }
    void setPosition(const glm::vec3& position) { this->position = position; }
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;
    virtual bool isOrtho() const = 0;

  protected:
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::mat4 projection = glm::mat4(1.0f);
};

class CameraOrtho : public Camera {
  public:
    CameraOrtho(const float& xmag, const float& ymag, const float& near, const float& far) : xmag(xmag), ymag(ymag), near(near), far(far) {}
    virtual ~CameraOrtho() = default;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override {
        float halfAspectRatio = ((float)width / (float)height) * 0.5f;
        xsize = xmag * halfAspectRatio;
        ysize = ymag * 0.5f;
        projection = glm::ortho(-xsize, xsize, -ysize, ysize, near, far);
    }
    virtual bool isOrtho() const override { return true; }
    const glm::vec2 getSize() const { return glm::vec2(xsize, ysize); }

  private:
    float xsize = 0.0, ysize = 0.0, xmag = 800.0f, ymag = 600.0f, near = 0.1f, far = 1000.0f;
};

class CameraPerspective : public Camera {
  public:
    CameraPerspective(const float& fov, const float& near, const float& far) : fov(fov), near(near), far(far) {}
    virtual ~CameraPerspective() = default;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override {
        projection = glm::perspective(glm::radians(fov), (float)width / (float)height, near, far);
    }
    virtual bool isOrtho() const override { return false; }

  private:
    float fov = CAMERA_MAX_FOV, near = 0.1f, far = 1000.0f;
};

struct CameraComponent {
    TagComponent tag;
    Camera* camera = nullptr;
    ViewProjection* vpo = nullptr;
    bool primary = true;
    bool fixedAspectRatio = false;
    CamKind camKind = CamKind::STATIC;
    float pitch = 0.0f, yaw = 90.0f, min = 0.5f, max = 1000.0f;
    glm::vec3 up = glm::vec3(0, 1, 0);
    CameraComponent() = default;
    CameraComponent(const CameraComponent& o) = default;
};
} // namespace Chimera
