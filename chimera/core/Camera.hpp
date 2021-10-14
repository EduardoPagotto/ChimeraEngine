#ifndef __CHIMERA_CAMERA_CONTROLER_HPP
#define __CHIMERA_CAMERA_CONTROLER_HPP

#include <glm/glm.hpp>

namespace Chimera {

class Camera {
  public:
    Camera() = default;
    Camera(const glm::mat4& projectionMatrix) : projectionMatrix(projectionMatrix) {}
    const glm::mat4 getProjectionMatrix() const { return projectionMatrix; }

    virtual ~Camera() = default;

    // TODO: implementar
    // void setPerspective();
    // void setOrtho();;

  protected:
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
};
} // namespace Chimera
#endif