#ifndef __CHIMERA_SCENE_CAMERA_ZZ1__HPP
#define __CHIMERA_SCENE_CAMERA_ZZ1__HPP

#include "chimera/core/Camera.hpp"

namespace Chimera {

class SceneCamera : public Camera {
  public:
    SceneCamera();
    SceneCamera(const glm::mat4& projectionMatrix) : Camera(projectionMatrix) {}
    virtual ~SceneCamera() = default;
    void setOrthographic(float size, float nearClip, float farClip);
    void setViewportSize(uint32_t width, uint32_t height);

  private:
    void recalculateProjection();

    float orthographicSize = 10.0f;
    float orthographicNear = -1.0f, orthographicFar = 1.0f;
    float aspectRatio = 1.0f;
};

} // namespace Chimera

#endif