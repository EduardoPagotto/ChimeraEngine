#ifndef EFFECT_H_
#define EFFECT_H_

#include <tinyxml2.h>

#include "Color.h"
#include "Entity.h"

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>

namespace Chimera {
namespace Graph {

typedef struct color {
            Color cambient;
            Color cdiffuse;
            Color cemission;
            Color cspecular;
            float shine;
            float trans;
} gcolors;
    
enum face{
    FRONT=GL_FRONT,
    BACK=GL_BACK,
    FRONT_BACK=GL_FRONT_AND_BACK
}; 

enum mode{
    AMBIENT=GL_AMBIENT,
    DIFFUSE=GL_DIFFUSE,
    EMISSION=GL_EMISSION,
    SPECULAR=GL_SPECULAR,
    SHININESS=GL_SHININESS,
    AMBIENT_AND_DIFFUSE=GL_AMBIENT_AND_DIFFUSE
};


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
        return this->gcolor.cambient;
    }
    
    Color getSpecular() const {
        return this->gcolor.cspecular;
    }
    Color getDiffuse() const {
        return this->gcolor.cdiffuse;
    }
    Color getEmissive() const {
        return this->gcolor.cemission;
    }

    float getShininess() const {
        return this->gcolor.shine;
    }
    
    float getTrans() const  {
        return this->gcolor.trans;
    }
    
    void setAmbient ( const Color &_color );
    void setSpecular ( const Color &_color );
    void setDiffuse ( const Color &_color );
    void setEmissive ( const Color &_color );
    void setShininess ( const float &_val );
    void setTrans(const float & _val);
    
    void setNameTextureId ( std::string _name ) {
        nameTextureId = _name;
    }

    std::string getNameTextureId() const {
        return nameTextureId;
    }

	void createDefaultEffect();

    void loadCollada ( tinyxml2::XMLElement* _nNode );

private:

    bool getPhong ( const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode );

    //Color diffuse;        /* Diffuse color RGBA */
    //Color ambient;        /* Ambient color RGB */
    //Color specular;       /* Specular 'shininess' */
    //Color emissive;       /* Emissive color RGB */

    //float shininess;
    std::string nameTextureId;
    
    face f;
    mode m;
    gcolors gcolor;
};
}
}

#endif
