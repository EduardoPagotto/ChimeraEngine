#ifndef __CHIMERA_TRANSFORMATION__HPP
#define __CHIMERA_TRANSFORMATION__HPP

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
// #include <glm/gtx/quaternion.hpp>
// #include "chimera/OpenGLDefs.hpp"
// #include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

class TransData {
  public:
    TransData() : model(glm::mat4(1.0f)), isStatic(false) {}
    virtual ~TransData() {}

    inline glm::vec3 getPosition() const { return glm::vec3(model[3]); }
    inline glm::vec3 getRotation() const { return glm::vec3(0.0, 0.0, 0.0); }
    inline glm::mat4 getMatrix() const { return model; }
    inline bool getStaticTranslation() const { return isStatic; }

    inline void setPosition(const glm::vec3& _pos) { model = glm::translate(model, _pos); }
    inline void setRotation(const glm::vec3& _rotation) {
        model = glm::eulerAngleYXZ(_rotation.y, _rotation.x, _rotation.z);
    }
    inline void setMatrix(const glm::mat4& _trans) { model = _trans; }
    void setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation);
    inline void setStaticTranslation(const bool& val) { isStatic = val; }

    inline glm::mat4 getModelMatrix(const glm::vec3& _position);

  private:
    bool isStatic;
    glm::mat4 model;
};
} // namespace Chimera
#endif
