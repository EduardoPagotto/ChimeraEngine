#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Color.h"
#include "Entity.h"
#include <map>
#include <tinyxml2.h>

#include "OpenGLDefs.h"
#include "Shader.h"
#include "TextureManager.h"

namespace Chimera {

#define SHADE_DIFFUSE "material.tDiffuse"
#define SHADE_SPECULA "material.tSpecular"
#define SHADE_EMISSIVE "material.tEmissive"

class Material : public Entity
{
public:
    Material ( std::string _name );
    virtual ~Material();

    virtual void init();

    void setAmbient(const Color &_color);
	void setSpecular(const Color &_color);
	void setDiffuse(const Color &_color);
	void setEmission(const Color &_color);
	void setShine(const float &_val);

    Color getAmbient() const;
    Color getSpecular() const;
    Color getDiffuse() const;
    Color getEmission() const;
    float getShine() const;
	bool isSet(const ModeMaterial &_val);
	void setFace(const FaceMaterial &_val);

    void apply();

    void createDefaultEffect();

    void loadCollada ( tinyxml2::XMLElement* root, tinyxml2::XMLElement* _nNode );

	void setTexDiffuse(Texture *_pTex) {
        mapTex[ SHADE_DIFFUSE ] = _pTex; //nome do shader usado como chave
	}

	void setTexSpecular(Texture *_pTex) {
        mapTex[ SHADE_SPECULA ] = _pTex;
	}

    void setTexEmission(Texture *_pTex) {
        mapTex[ SHADE_EMISSIVE ] = _pTex;
	}

	Texture *getTexDiffuse() {
        return mapTex[ SHADE_DIFFUSE ];
	}

	bool hasTexture() {
		return mapTex.size() > 0 ? true : false;
	}

private:
    void loadColladaProfile(tinyxml2::XMLElement* _nNode);

    bool getPhong ( const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode );

    Color diffuse;        /* Diffuse color RGBA */
    Color ambient;        /* Ambient color RGB */
    Color specular;       /* Specular 'shininess' */
    Color emission;       /* Emissive color RGB */
	float shine;

    int tipoTexturasDisponiveis;

    FaceMaterial faceMaterial;

    std::map<ModeMaterial, bool> map_modes;
	std::map<ModeMaterial, void*> map_params;

    std::map<std::string, Texture*> mapTex;

    TextureManager *texManager;
	Shader *shader;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
