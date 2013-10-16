#ifndef EFFECT_H_
#define EFFECT_H_

#include "Color.h"
#include "Node.h"

namespace Chimera {

class Effect : public Node {
public:
	
    Effect() : Node(EntityKind::EFFECT),  shininess(10.5f)  {}

	Effect(const Effect& _cpy) :Node(_cpy) {
        
		diffuse = _cpy.diffuse;
		ambient = _cpy.ambient;
		specular = _cpy.specular;
		emissive = _cpy.emissive;
		shininess = _cpy.shininess;
        
		nameTextureId = _cpy.nameTextureId;
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
    
    void setAmbient(Color _color) {
        ambient = _color;
    }
    
    void setSpecular(Color _color) {
        specular = _color;
    }
    
    void setDiffuse(Color _color) {
        diffuse = _color;
    }	

    void setEmissive(Color _color) {
        emissive = _color;
    }    
    
    float getShininess() const {
        return shininess;
    }
    
    void setShininess(float _val) {
        shininess = _val;
    }
    
	void setNameTextureId(std::string _name) {
        nameTextureId = _name;
    }

    std::string getNameTextureId() const {
        return nameTextureId;
    }
    
private:
    
    Color diffuse;        /* Diffuse color RGBA */
    Color ambient;        /* Ambient color RGB */
    Color specular;       /* Specular 'shininess' */
    Color emissive;       /* Emissive color RGB */
    
    float shininess;
    
    std::string nameTextureId;
};

}

#endif