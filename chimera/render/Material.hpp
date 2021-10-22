#ifndef __CHIMERA_MATERIAL__HPP
#define __CHIMERA_MATERIAL__HPP

#include "chimera/core/Texture.hpp"
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
#define SHADE_TEXTURE_DIFFUSE "material.tDiffuse"
#define SHADE_TEXTURE_SPECULA "material.tSpecular"
#define SHADE_TEXTURE_EMISSIVE "material.tEmissive"

class Material {
  public:
    Material();
    virtual ~Material();
    void init();
    void setDefaultEffect();
    void addTexture(const std::string& uniformTexName, Texture* texture);
    inline void setAmbient(const glm::vec4& _color) { listMaterial.push_back(UniformVal(SHADE_MAT_AMBIENTE, _color)); }
    inline void setSpecular(const glm::vec4& _color) { listMaterial.push_back(UniformVal(SHADE_MAT_SPECULA, _color)); }
    inline void setDiffuse(const glm::vec4& _color) { listMaterial.push_back(UniformVal(SHADE_MAT_DIFFUSE, _color)); }
    inline void setEmission(const glm::vec4& _color) { // listMaterial.push_back( UniformVal(SHADE_MAT_EMISSIVE, _color));
    }
    inline void setShine(const float& _val) { listMaterial.push_back(UniformVal(SHADE_MAT_SHININESS, _val)); }

    bool hasTexture() { return !mapTex.empty(); }
    void bindMaterialInformation(Shader* _shader);
    void bindMaterialInformation(std::vector<UniformVal>& uniforms);

  private:
    int tipoTexturasDisponiveis;
    std::map<std::string, Texture*> mapTex;
    std::list<UniformVal> listMaterial;
};
} // namespace Chimera
#endif