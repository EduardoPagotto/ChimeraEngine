#pragma once
#include <glm/glm.hpp>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

struct EyeMatrix { // 0=centro; 1=left; 2=right;
    glm::mat4 view;
    glm::mat4 viewProjection;
    glm::mat4 viewProjectionInverse;
};

enum class EyeIndex { center = 0, left = 1, right = 2 };

class EyeView {
  public:
    EyeView() : index(0) {}
    void setIndex(const EyeIndex& index) { this->index = (uint8_t)index; }
    const EyeIndex getIndex() const { return (EyeIndex)index; }
    const glm::mat4& getView() const { return matrix[index].view; }
    const glm::mat4& getViewProjection() const { return matrix[index].viewProjection; };
    const glm::mat4& getViewProjectionInverse() const { return matrix[index].viewProjectionInverse; }

    void update(const glm::mat4& view, const glm::mat4& projection) {
        matrix[index].view = view;
        matrix[index].viewProjection = projection * view;
        matrix[index].viewProjectionInverse = glm::inverse(view) * glm::inverse(projection);
    }

  private:
    uint8_t index;
    EyeMatrix matrix[3]; // center; left; right;
};

class ICamera {
  public:
    virtual ~ICamera() {}
    virtual const glm::mat4& getViewMatrix() const = 0;
    virtual const glm::mat4& getProjectionMatrix() const = 0;
    virtual const glm::mat4& getViewProjectionMatrix() const = 0;
    virtual const glm::mat4& getViewProjectionMatrixInverse() const = 0;
    virtual const glm::mat4& recalculateMatrix(const uint8_t& eyeIndex) = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void onUpdate(const double& ts) = 0;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;
    virtual const bool is3D() const = 0;
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
