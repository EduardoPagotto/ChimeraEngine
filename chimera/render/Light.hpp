#pragma once
#include "chimera/render/Shader.hpp"

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
    Light() : type(LightType::POSITIONAL) {} // TODO: muito a fazer!!!! indice e luz necessario para o shader
    virtual ~Light() {}
    inline void setAmbient(const glm::vec4& color) { listProp.push_back(UValue(SHADE_LIGHT_AMBIENT, color)); }
    inline void setSpecular(const glm::vec4& color) { listProp.push_back(UValue(SHADE_LIGHT_SPECULAR, color)); }
    inline void setDiffuse(const glm::vec4& color) { listProp.push_back(UValue(SHADE_LIGHT_DIFFUSE, color)); }
    inline void setType(const LightType& type) { this->type = type; }
    inline void bindLight(std::vector<UValue>& uniforms, const glm::mat4& mat) {
        uniforms.push_back(UValue(SHADE_LIGHT_POSITION, glm::vec3(mat[3])));
        copy(listProp.begin(), listProp.end(), back_inserter(uniforms));
    }

  private:
    // int number;
    LightType type;
    std::vector<UValue> listProp;
};
} // namespace Chimera
