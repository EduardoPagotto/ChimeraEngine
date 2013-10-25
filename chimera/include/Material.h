#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>
#include "Texture.h"
#include "Effect.h"

namespace Chimera {

class Material : Node {
public:

    friend class Loader;

    Material();
    Material ( const Material &_cpy );
    virtual ~Material();
    virtual void update ( DataMsg *dataMsg );
private:
    
    Texture *pTextura;
    Effect *pEffect;
    
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
