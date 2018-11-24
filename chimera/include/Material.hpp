#ifndef __CHIMERA_MATERIAL__HPP
#define __CHIMERA_MATERIAL__HPP

#include "Color.hpp"
#include "Entity.hpp"
#include <map>
#include <tinyxml2.h>

#include "OpenGLDefs.hpp"
#include "Shader.hpp"
#include "TextureManager.hpp"

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

    void setAmbient(const Color& _color);
    void setSpecular(const Color& _color);
    void setDiffuse(const Color& _color);
    void setEmission(const Color& _color);
    void setShine(const float& _val);

    Color getAmbient() const;
    Color getSpecular() const;
    Color getDiffuse() const;
    Color getEmission() const;
    float getShine() const;

    void apply(Shader* _shader);
    void createDefaultEffect();

    void defineTextureByIndex(const unsigned int& _serial);

    void loadTextureFromFile(const std::string& _nome, const TEX_SEQ& _seq,
                             const std::string& _arquivo);

    bool hasTexture() { return mapTex.size() > 0 ? true : false; }

  private:
    Color diffuse;  /* Diffuse color RGBA */
    Color ambient;  /* Ambient color RGB */
    Color specular; /* Specular 'shininess' */
    Color emission; /* Emissive color RGB */
    float shine;

    int tipoTexturasDisponiveis;

    std::map<std::string, void*> mapMatVal;
    std::map<std::string, Texture*> mapTex;

    TextureManager* texManager;
};
} // namespace Chimera
#endif
