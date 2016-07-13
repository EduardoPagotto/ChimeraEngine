#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Color.h"
#include "OpenGLDefs.h"
#include "Entity.h"
#include <map>
#include <tinyxml2.h>


namespace Chimera {
    
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
    Color getSpecular() const ;
    Color getDiffuse() const;
    Color getEmission() const;
    float getShine() const;
	bool isSet(const ModeMaterial &_val);
	void setFace(const FaceMaterial &_val);
    
    void apply(bool hasTexture);
    
    void createDefaultEffect();
    
    void loadCollada ( tinyxml2::XMLElement* root, tinyxml2::XMLElement* _nNode );
    
private:
    
    void loadColladaProfile(tinyxml2::XMLElement* _nNode);
    
    bool getPhong ( const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode );
    
    Color diffuse;        /* Diffuse color RGBA */
    Color ambient;        /* Ambient color RGB */
    Color specular;       /* Specular 'shininess' */
    Color emission;       /* Emissive color RGB */
	float shine;
    
    FaceMaterial faceMaterial;
    
    std::map<ModeMaterial, bool> map_modes;
	std::map<ModeMaterial, void*> map_params;

};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
