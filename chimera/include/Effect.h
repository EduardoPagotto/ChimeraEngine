#ifndef EFFECT_H_
#define EFFECT_H_

#include <GL/gl.h>
#include <string>
#include "Color.h"
#include "Node.h"

namespace Chimera {

class Effect : public Node {
public:
	
    Effect(std::string _id, std::string _name);

	Effect(const Effect& _cpy);

	virtual ~Effect() {}
	
	virtual void update ( DataMsg *dataMsg );
	
	void render();
	
	Color getAmbient() const { return ambient; }
    Color getSpecular() const { return specular; }
    Color getDiffuse() const { return diffuse; }
    Color getEmissive() const { return emissive; }    
    
    void setAmbient(Color _color) { ambient = _color;}
    void setSpecular(Color _color) { specular = _color;}
    void setDiffuse(Color _color) { diffuse = _color; }	
    void setEmissive(Color _color) { emissive = _color;}    
    
    float getShininess() const { return shininess;}
    void setShininess(float _val) {  shininess = _val;}
	void setNameTextureId(std::string _name) { nameTextureId = _name; }

    std::string getNameTextureId() const { return nameTextureId; }
    
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