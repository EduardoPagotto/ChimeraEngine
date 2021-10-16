#ifndef __CHIMERA_TRANSFORMATION__HPP
#define __CHIMERA_TRANSFORMATION__HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Chimera {
class Transform {
  public:
    Transform() = default;
    Transform(const glm::mat4& transform) : transform(transform) {}
    virtual ~Transform() = default;

    virtual glm::vec3 getPosition() { return glm::vec3(this->transform[3]); }
    virtual glm::vec3 getRotation() { return glm::vec3(0.0, 0.0, 0.0); } // TODO: Implementar
    virtual glm::mat4 getMatrix() { return this->transform; }
    virtual glm::mat4 getModelMatrix(const glm::vec3& _position); // TODO: SINISTRO!!!!!

    virtual void setPosition(const glm::vec3& pos) { this->transform = glm::translate(this->transform, pos); }
    virtual void setRotation(const glm::vec3& rot) { transform = glm::eulerAngleYXZ(rot.y, rot.x, rot.z); }
    virtual void setMatrix(const glm::mat4& transform) { this->transform = transform; }
    virtual void init(const glm::vec3& size) {} // TODO: Implementar

    glm::vec3 translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    glm::mat4 getNewTransform() const {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotation.x, {1, 0, 0}) * glm::rotate(glm::mat4(1.0f), rotation.y, {0, 1, 0}) *
                        glm::rotate(glm::mat4(1.0f), rotation.z, {0, 0, 1});
        return glm::translate(glm::mat4(1.0f), translation) * rot * glm::scale(glm::mat4(1.0f), scale);
    }

  private:
    glm::mat4 transform = glm::mat4(1.0f);
};
} // namespace Chimera
#endif
