#ifndef LIGHT_H_
#define LIGHT_H_

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
#include "Color.h"
#include "Node.h"
#include "Transform.h"

namespace Chimera {

enum class LightType {
    POINT=0,
    SPOT,
    DIRECTIONAL
};

class Light : public Node {
public:
    Light ();
    
    Light (const Light& _light );
    
    virtual ~Light();

    virtual void update ( DataMsg *_dataMsg );
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
    
    void setAmbient(Color _color) {
       ambient = _color;
    }

    void setSpecular(Color _color) {
        specular = _color;
    }
    
    void setDiffuse(Color _color) {
       diffuse = _color;
    }
    
private:

    Color ambient;
    Color specular;
    Color diffuse;
    
    int number;
    LightType type;
};

}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
