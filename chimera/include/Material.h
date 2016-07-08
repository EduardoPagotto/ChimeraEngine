#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>
#include "Texture.h"
#include "Effect.h"

namespace Chimera {
namespace Graph {
    
class Material : public Entity
{
public:

    friend class Loader;

    Material ( std::string _name );
    Material ( const Material &_cpy );
    virtual ~Material();

    //virtual void update ( DataMsg *dataMsg );

    virtual void clone ( Entity **ppNode );

    Texture *getTexture() const {
        return pTexture;
    }

    Effect *getEffect() const {
        return pEffect;
    }

	void init();

    void begin ( bool _texture );
    void end();

    //FIXME manter privado
    Effect *pEffect;
    Texture *pTexture;
private:
    bool hasTextureAtive;
};
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
