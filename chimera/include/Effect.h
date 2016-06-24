#ifndef EFFECT_H_
#define EFFECT_H_

#include <tinyxml2.h>

#include "Color.h"
#include "Entity.h"

namespace Chimera {
namespace Graph {

class Effect : public Entity
{
public:

    Effect ( std::string _id, std::string _name );

    Effect ( const Effect& _cpy );

    virtual ~Effect() {}

    //virtual void update ( DataMsg *dataMsg );

    virtual void clone ( Entity **ppNode );

    void init();

    void apply();

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

    void setAmbient ( const Color &_color ) {
        ambient = _color;
    }
    void setSpecular ( const Color &_color ) {
        specular = _color;
    }
    void setDiffuse ( const Color &_color ) {
        diffuse = _color;
    }
    void setEmissive ( const Color &_color ) {
        emissive = _color;
    }

    float getShininess() const {
        return shininess;
    }
    void setShininess ( const float &_val ) {
        shininess = _val;
    }
    void setNameTextureId ( std::string _name ) {
        nameTextureId = _name;
    }

    std::string getNameTextureId() const {
        return nameTextureId;
    }

    void loadCollada ( tinyxml2::XMLElement* _nNode );

private:

    bool getPhong ( const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode );

    Color diffuse;        /* Diffuse color RGBA */
    Color ambient;        /* Ambient color RGB */
    Color specular;       /* Specular 'shininess' */
    Color emissive;       /* Emissive color RGB */

    float shininess;
    std::string nameTextureId;
};
}
}

#endif
