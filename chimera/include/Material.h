#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library

#include <string>

#include "Effect.h"
#include "Image.h"

namespace Chimera {

class Material {
public:

    friend class Loader; 
    
    Material();
    Material ( const Material &_cpy );
    virtual ~Material();

    void exec ();

    int link ();

    void initTex ();

private:
    Image *pTexture;
    Effect effect;
    unsigned int idTex;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
