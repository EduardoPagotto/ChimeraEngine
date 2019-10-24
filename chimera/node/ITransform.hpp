#ifndef __CHIMERA_COORD__HPP
#define __CHIMERA_COORD__HPP

#include "glm/glm.hpp"

namespace Chimera {

class ITransform {
  public:
    ITransform() {}
    virtual ~ITransform() noexcept {}

    virtual glm::vec3 getPosition() = 0;
    virtual void setPosition(const glm::vec3& _pos) = 0;

    virtual glm::vec3 getRotation() = 0;
    virtual void setRotation(const glm::vec3& _rot) = 0;

    virtual glm::mat4 getMatrix() = 0;
    virtual void setMatrix(const glm::mat4& _trans) = 0;

    virtual void init(const glm::vec3& _size) = 0;
    virtual glm::mat4 getModelMatrix(const glm::vec3& _pos) = 0;
};
} // namespace Chimera

#endif