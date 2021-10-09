#ifndef __CHIMERA_ICAMERA_ZZ_HPP
#define __CHIMERA_ICAMERA_ZZ_HPP

#include <glm/glm.hpp>

namespace Chimera {

class ICamera {
  public:
    virtual ~ICamera() {}
    virtual glm::mat4 getViewMatrix() = 0;
    virtual glm::mat4 getProjectionMatrix(const glm::ivec2& res) = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual const glm::vec3& getFront() const = 0;
    virtual const glm::vec3& getUp() const = 0;

    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void invertPitch() = 0;
};
} // namespace Chimera
#endif