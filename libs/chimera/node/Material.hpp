#ifndef __CHIMERA_MATERIAL__HPP
#define __CHIMERA_MATERIAL__HPP

#include <list>
#include <map>

#include "Entity.hpp"
#include "TextureManager.hpp"
#include "chimera/core/Color.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/ShaderValue.hpp"
#include "chimera/core/Tex.hpp"

namespace Chimera {

#define SHADE_TEXTURE_DIFFUSE "material.tDiffuse"
#define SHADE_TEXTURE_SPECULA "material.tSpecular"
#define SHADE_TEXTURE_EMISSIVE "material.tEmissive"
#define SHADE_TEXTURE_SELETOR_TIPO_VALIDO "tipo"

#define SHADE_MAT_AMBIENTE "material.ambient"
#define SHADE_MAT_DIFFUSE "material.diffuse"
#define SHADE_MAT_SPECULA "material.specular"
#define SHADE_MAT_EMISSIVE "material.emissive"
#define SHADE_MAT_SHININESS "material.shininess"

class Material : public Entity {
  public:
    Material(std::string _name);
    virtual ~Material();

    virtual void init();

    inline void setAmbient(const Color& _color) {
        listMaterial.push_back(new ShaderValue4vf(SHADE_MAT_AMBIENTE, _color.get()));
    }

    inline void setSpecular(const Color& _color) {
        listMaterial.push_back(new ShaderValue4vf(SHADE_MAT_SPECULA, _color.get()));
    }

    inline void setDiffuse(const Color& _color) {
        listMaterial.push_back(new ShaderValue4vf(SHADE_MAT_DIFFUSE, _color.get()));
    }

    inline void setEmission(const Color& _color) { // TODO implementar
        // listMaterial.push_back(new ShaderValue4vf(SHADE_MAT_EMISSIVE, _color.ptr()));
    }

    inline void setShine(const float& _val) { listMaterial.push_back(new ShaderValue1vf(SHADE_MAT_SHININESS, _val)); }

    void apply(Shader* _shader);
    void createDefaultEffect();
    void setTexture(Texture* _pTex);
    void loadTextureFromFile(const std::string& _nome, const TEX_KIND& _seq, const std::string& _arquivo);
    bool hasTexture() { return mapTex.size() > 0 ? true : false; }

  private:
    int tipoTexturasDisponiveis;
    std::list<ShaderValue*> listMaterial;
    std::map<std::string, Texture*> mapTex;
    TextureManager* texManager;
};
} // namespace Chimera
#endif
