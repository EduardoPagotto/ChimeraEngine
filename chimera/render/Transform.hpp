#ifndef __CHIMERA_TRANSFORMATION__HPP
#define __CHIMERA_TRANSFORMATION__HPP

#include <glm/glm.hpp>

namespace Chimera {
class Transform {
  public:
    Transform();
    virtual ~Transform();
    virtual glm::vec3 getPosition();
    virtual void setPosition(const glm::vec3& _pos);
    virtual glm::vec3 getRotation();
    virtual void setRotation(const glm::vec3& _rotation);
    virtual glm::mat4 getMatrix();
    virtual void setMatrix(const glm::mat4& _trans);
    virtual glm::mat4 getModelMatrix(const glm::vec3& _position);
    virtual void init(const glm::vec3& _size);

  private:
    glm::mat4 model;
};
} // namespace Chimera
#endif
