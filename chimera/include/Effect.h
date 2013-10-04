#ifndef EFFECT_H_
#define EFFECT_H_

#include "Color.h"

namespace Chimera {

class Effect {
public:
	
    Effect() :shininess(10.5f)  {}

	Effect(const Effect& _cpy) {
		diffuse = _cpy.diffuse;
		ambient = _cpy.ambient;
		specular = _cpy.specular;
		emissive = _cpy.emissive;
		shininess = _cpy.shininess;
		nameTexture = _cpy.nameTexture;
	}

	virtual ~Effect() {}

	Color getAmbient() const {
        return ambient;
    }
    
    Color getSpecular() const {
        return specular;
    }
    
    Color getDiffuse() const {
        return diffuse;
    }

    Color getEmissive() const {
        return emissive;
    }    
    
    void setAmbient(Color color) {
        ambient = color;
    }
    
    void setSpecular(Color color) {
        specular = color;
    }
    
    void setDiffuse(Color color) {
        diffuse = color;
    }	

    void setEmissive(Color color) {
        emissive = color;
    }    
    
    float getShininess() const {
        return shininess;
    }
    
    void setShininess(float val) {
        shininess = val;
    }
    
	void setNameTexture(std::string _name) {
        nameTexture = _name;
    }

    std::string getNameTexture() const {
        return nameTexture;
    }
    
	
    
private:
    
    Color diffuse;        /* Diffuse color RGBA */
    Color ambient;        /* Ambient color RGB */
    Color specular;       /* Specular 'shininess' */
    Color emissive;       /* Emissive color RGB */
    
    float shininess;
    
    std::string nameTexture;
};

}

#endif