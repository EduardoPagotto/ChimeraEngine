#ifndef __CHIMERA_MATERIAL__HPP
#define __CHIMERA_MATERIAL__HPP

#include "chimera/core/ShaderValue.hpp"
#include "chimera/render/Tex.hpp"
#include <glm/glm.hpp>
#include <list>
#include <map>

namespace Chimera {

#define SHADE_TEXTURE_SELETOR_TIPO_VALIDO "tipo"

#define SHADE_MAT_AMBIENTE "material.ambient"
#define SHADE_MAT_DIFFUSE "material.diffuse"
#define SHADE_MAT_SPECULA "material.specular"
#define SHADE_MAT_EMISSIVE "material.emissive"
#define SHADE_MAT_SHININESS "material.shininess"

class Material {
  public:
    Material();
    virtual ~Material();

    void init();

    void setDefaultEffect();

    void addTexture(TexImg* _pTex);

    inline void setAmbient(const glm::vec4& _color) {
        listMaterial.push_back(new ShaderValue4vf(SHADE_MAT_AMBIENTE, _color));
    }

    inline void setSpecular(const glm::vec4& _color) {
        listMaterial.push_back(new ShaderValue4vf(SHADE_MAT_SPECULA, _color));
    }

    inline void setDiffuse(const glm::vec4& _color) {
        listMaterial.push_back(new ShaderValue4vf(SHADE_MAT_DIFFUSE, _color));
    }

    inline void setEmission(const glm::vec4& _color) { // TODO implementar
        // listMaterial.push_back(new ShaderValue4vf(SHADE_MAT_EMISSIVE, _color));
    }

    bool hasTexture() { return listTex.size() > 0 ? true : false; }

    inline void setShine(const float& _val) { listMaterial.push_back(new ShaderValue1vf(SHADE_MAT_SHININESS, _val)); }

    void apply(Shader* _shader);

  private:
    int tipoTexturasDisponiveis;
    std::list<TexImg*> listTex;
    std::list<ShaderValue*> listMaterial;
};
} // namespace Chimera
#endif