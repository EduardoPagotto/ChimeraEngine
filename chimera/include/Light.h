#ifndef LIGHT_H_
#define LIGHT_H_

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
#include "Color.h"
#include "Transform.h"

namespace Chimera {

enum class LightType {
    POINT=0,
    SPOT,
    DIRECTIONAL
};

class Light : public Transform {
public:
    Light ( LightType _type, int number, std::string id, std::string _name );
    Light (const Light& light );
    Light ( );
    
    virtual ~Light();

    virtual void update ( DataMsg *dataMsg );
    virtual void exec();

    Color getAmbient() {
        return ambient;
    }

    Color getSpecular() {
        return specular;
    }
    
    Color getDiffuse() {
        return diffuse;
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
    
private:

    Color ambient;
    Color specular;
    Color diffuse;
    
    int number;
    LightType typeLight;
};

}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
