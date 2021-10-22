#ifndef __CHIMERA_LIGHT__HPP
#define __CHIMERA_LIGHT__HPP

#include "chimera/core/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Chimera {

#define SHADE_LIGHT_POSITION "light.position"
#define SHADE_LIGHT_AMBIENT "light.ambient"
#define SHADE_LIGHT_DIFFUSE "light.diffuse"
#define SHADE_LIGHT_SPECULAR "light.specular"

enum class LightType {
    DIRECTIONAL = 0,
    POSITIONAL = 1,
};

class Light {
  public:
    Light();
    virtual ~Light();

    void bindLightInformation(Shader* _pShader);
    void bindLightInformation(std::vector<UniformVal>& uniforms);
    inline void setAmbient(const glm::vec4& color) { listProp.push_back(UniformVal(SHADE_LIGHT_AMBIENT, color)); }
    inline void setSpecular(const glm::vec4& color) { listProp.push_back(UniformVal(SHADE_LIGHT_SPECULAR, color)); }
    inline void setDiffuse(const glm::vec4& color) { listProp.push_back(UniformVal(SHADE_LIGHT_DIFFUSE, color)); }
    inline void setPosition(const glm::vec3& pos) {
        listProp.push_back(UniformVal(SHADE_LIGHT_POSITION, pos));
        transform = glm::translate(transform, pos); // FIXME: ver se da para remover e passar para shader
    }
    inline void setType(const LightType& type) { this->type = type; }
    inline void setTransform(const glm::mat4& trans) { transform = trans; }
    inline glm::vec3 getPosition() const { return glm::vec3(transform[3]); }
    inline void setRotation(const glm::vec3& rotation) { transform = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z); }

  private:
    int number;
    LightType type;
    glm::mat4 transform;
    std::vector<UniformVal> listProp;
};
} // namespace Chimera
#endif
