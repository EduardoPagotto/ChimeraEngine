#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>
#include "Texture.h"
#include "Effect.h"

namespace Chimera {
namespace Graph {
    
class Material : public Node
{
public:

    friend class Loader;

    Material ( std::string _id, std::string _name );
    Material ( const Material &_cpy );
    virtual ~Material();

    virtual void update ( DataMsg *dataMsg );

    virtual void clone ( Node **ppNode );

    Texture *getTexture() const {
        return pTextura;
    }

    Effect *getEffect() const {
        return pEffect;
    }

    void createDefaultEffect();

    void begin ( bool _texture );

    void end();

    //FIXME manter privado
    Effect *pEffect;
    Texture *pTextura;
private:

    bool hasTextureAtive;
    
    

};
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
