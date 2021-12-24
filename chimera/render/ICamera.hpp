#pragma once
#include <glm/glm.hpp>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f
#define EYE_CENTER 0
#define EYE_LEFT 1
#define EYE_RIGHT 2

class EyeView {
  public:
    EyeView() : index(0), noseDist(0.4f) {}
    void setIndex(const uint8_t& index) { this->index = index; }
    const uint8_t getIndex() const { return index; }
    const glm::mat4& getView() const { return view[index]; }
    const glm::mat4& getViewProjection() const { return viewProjection[index]; }
    const glm::mat4& getViewProjectionInverse() const { return viewProjectionInverse[index]; }
    const float& getNoseDist() const { return noseDist; }

    void update(const glm::mat4& view, const glm::mat4& projection) {
        this->view[index] = view;
        this->viewProjection[index] = projection * view;
        this->viewProjectionInverse[index] = glm::inverse(view) * glm::inverse(projection);
    }

  private:
    uint8_t index;
    float noseDist;
    glm::mat4 view[3], viewProjection[3], viewProjectionInverse[3]; // 0=centro; 1=left; 2=right;
};

class ICamera {
  public:
    virtual ~ICamera() {}
    virtual const glm::mat4& getProjectionMatrix() const = 0;
    virtual const glm::mat4& recalculateMatrix(const uint8_t& eyeIndex) = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void onUpdate(const double& ts) = 0;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;
    virtual const bool is3D() const = 0;
    virtual EyeView* view() = 0;
};

class ICamera3D : public ICamera {
  public:
    virtual ~ICamera3D() {}
    virtual const glm::vec3& getFront() const = 0;
    virtual const glm::vec3& getUp() const = 0;
    virtual float updateDistanceFront() = 0;
    virtual void invertPitch() = 0;
    virtual void setFov(const float& value) = 0;
    virtual void setNear(const float& value) = 0;
    virtual void setFar(const float& value) = 0;
    virtual void updateVectors() = 0;
};
} // namespace Chimera
