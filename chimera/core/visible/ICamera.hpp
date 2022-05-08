#pragma once
#include "chimera/core/TagComponent.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

struct EyeMat {
    glm::mat4 view, viewProjection, viewProjectionInverse;
    void update(const glm::mat4& _view, const glm::mat4& _projection) {
        view = _view;
        viewProjection = _projection * _view;
        viewProjectionInverse = glm::inverse(_view) * glm::inverse(_projection);
    }
};

class EyeView {
  public:
    EyeView() : index(0), noseDist(0.4f) {}
    void setIndex(const uint8_t& index) { this->index = index; }
    const uint8_t getIndex() const { return index; }
    const glm::mat4& getView() const { return head[index].view; }
    const glm::mat4& getViewProjection() const { return head[index].viewProjection; }
    const glm::mat4& getViewProjectionInverse() const { return head[index].viewProjectionInverse; }
    const float& getNoseDist() const { return noseDist; }
    void update(const glm::mat4& view, const glm::mat4& projection) { head[index].update(view, projection); }
    void create() { head.push_back(EyeMat()); }
    const uint32_t size() const { return head.size(); }
    std::vector<EyeMat>& getHead() { return head; }

  private:
    uint8_t index;
    float noseDist;
    std::vector<EyeMat> head;
};

class ICamera {
  public:
    virtual ~ICamera() {}
    virtual const glm::mat4& getProjection() const = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void onUpdate(const double& ts) = 0;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;
    virtual const bool is3D() const = 0;
    virtual EyeView* view() = 0;
};

struct CameraComponent {
    TagComponent tag;
    ICamera* camera = nullptr;
    bool primary = true;
    bool fixedAspectRatio = false;
    bool single = true; // FIXME: Camera simples ou dupla aqui, melhorar para o arquivo .DAE
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    // CameraComponent(glm::mat4 projection) : camera(projection) {}
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
