#pragma once
#include "UValue.hpp"
#include "chimera/core/TagComponent.hpp"

namespace ce {

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
    inline void setAmbient(const glm::vec4& color) { listProp[SHADE_LIGHT_AMBIENT] = UValue(color); }
    inline void setSpecular(const glm::vec4& color) { listProp[SHADE_LIGHT_SPECULAR] = UValue(color); }
    inline void setDiffuse(const glm::vec4& color) { listProp[SHADE_LIGHT_DIFFUSE] = UValue(color); }
    inline void setType(const LightType& type) { this->type = type; }
    inline void bindLight(MapUniform& uniforms, const glm::mat4& mat) {
        uniforms[SHADE_LIGHT_POSITION] = UValue(glm::vec3(mat[3]));
        uniforms.insert(listProp.begin(), listProp.end());
    }

  private:
    // int number;
    LightType type;
    MapUniform listProp;
};

struct LightComponent {
    TagComponent tag;
    Light* light = new Light();
    bool global = true;
    LightComponent() = default;
};

} // namespace ce
