#ifndef __CHIMERA_CAMERA_CONTROLER_HPP
#define __CHIMERA_CAMERA_CONTROLER_HPP

#include <glm/glm.hpp>

namespace Chimera {

class Camera {
  public:
    Camera(const glm::mat4& projectionMatrix) : projectionMatrix(projectionMatrix) {}
    const glm::mat4 getProjectionMatrix() const { return projectionMatrix; }

    // TODO: implementar
    // void setPerspective();
    // void setOrtho();;

  private:
    glm::mat4 projectionMatrix;
};
} // namespace Chimera
#endif