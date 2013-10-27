#ifndef LIGHT_H_
#define LIGHT_H_

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include "Color.h"
#include "Node.h"

namespace Chimera {

enum class LightType {
    POINT=0,
    SPOT,
    DIRECTIONAL
};

class Light : public Node {
public:

    friend class Loader;

    Light ( LightType _lightType, std::string _id, std::string _name );

    Light ( const Light& _light );

    virtual ~Light();

    virtual void update ( DataMsg *_dataMsg );

    virtual void clone ( Node **ppNode );

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

    void setAmbient ( Color _color ) {
        ambient = _color;
    }

    void setSpecular ( Color _color ) {
        specular = _color;
    }

    void setDiffuse ( Color _color ) {
        diffuse = _color;
    }

    void setType ( LightType _type ) {
        type = _type;
    }

    void setPositionRotation(btVector3 _posicao, btVector3 _rotation);
    
private:

    btTransform transform;
    btVector3 position;

    Color ambient;
    Color specular;
    Color diffuse;

    int number;
    LightType type;
};

}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
