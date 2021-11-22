#pragma once
#include <glm/glm.hpp>

namespace Chimera {
class ITrans {
  public:
    virtual ~ITrans() = default;
    virtual const glm::vec3 getPosition() const = 0;
    virtual const glm::mat4 getMatrix() const = 0;
    virtual const glm::mat4 translateSrc(const glm::vec3& pos) const = 0;
    // virtual const glm::vec3 getRotation() = 0; // TODO: Implementar
    virtual void setPosition(const glm::vec3& pos) = 0;
    virtual void setRotation(const glm::vec3& rot) = 0;
    virtual void setMatrix(const glm::mat4& transform) = 0;
};
} // namespace Chimera