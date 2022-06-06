#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

enum class CamKind { FPS = 0, ORBIT = 1, STATIC = 3 };

struct CameraControllerData {
    CameraControllerData() = default;
    CameraControllerData(const CameraControllerData& o) = default;
    CamKind camKind = CamKind::STATIC;
    bool perspectiove = false;
    float pitch = 0.0f, yaw = 90.0f, movementSpeed = 1.0f, distance = 1.0f, min = 0.5f, max = 1000.0f;
    // glm::vec3 position = glm::vec3(1, 0, 0)
    glm::vec3 front = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0), right = glm::vec3(1, 0, 0);
    glm::vec3 worldUp = glm::vec3(0, 1, 0); // FIXME: Remover ???
};

class Projection {
  public:
    virtual void setViewportProjection(const uint32_t& width, const uint32_t& height) = 0;
    virtual const glm::mat4& getProjection() const = 0;
};

class Orthogonal : public Projection {
  public:
    Orthogonal() = default;
    Orthogonal(const Orthogonal& o) = default;
    Orthogonal(const float& xmag, const float& ymag, const float& near, const float& far) : xmag(xmag), ymag(ymag), near(near), far(far){};
    virtual void setViewportProjection(const uint32_t& width, const uint32_t& height) override {
        float halfAspectRatio = (float)width / (float)height * 0.5f;
        float xsize = xmag * halfAspectRatio;
        float ysize = ymag * 0.5f;
        projection = glm::ortho(-xsize, xsize, -ysize, ysize, near, far);
    }
    virtual const glm::mat4& getProjection() const override { return projection; }

  private:
    float xmag = 800;
    float ymag = 600;
    float near = 0.1f;
    float far = 1000.0f;
    glm::mat4 projection = glm::mat4(1.0f);
};

class Perspective : public Projection {
  public:
    Perspective() = default;
    Perspective(const Perspective& o) = default;
    Perspective(const float& fov, const float& near, const float& far) : fov(fov), near(near), far(far) {}
    virtual void setViewportProjection(const uint32_t& width, const uint32_t& height) override {
        projection = glm::perspective(glm::radians(fov), (float)width / (float)height, near, far);
    }
    virtual const glm::mat4& getProjection() const override { return projection; }

  private:
    float near = 0.1f;
    float far = 1000.0f;
    float fov = CAMERA_MAX_FOV;
    glm::mat4 projection = glm::mat4(1.0f);
};
} // namespace Chimera