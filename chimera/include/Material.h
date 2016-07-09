#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>
#include "Texture.h"
#include "Effect.h"

namespace Chimera {
    
class Material : public Entity
{
public:

    friend class Loader;
	friend class LoaderDae;

    Material ( std::string _name );
    Material ( const Material &_cpy );
    virtual ~Material();

    Texture *getTexture() const {
        return pTexture;
    }

    Effect *getEffect() const {
        return pEffect;
    }

	virtual void init();

    void begin ( bool _texture );
    void end();

	void setEffect(Effect *_pEffect) {
		pEffect = _pEffect;
	}

	void setTexture(Texture* _pTexture) {
		pTexture = _pTexture;
	}

private:

	Effect *pEffect;
	Texture *pTexture;

    bool hasTextureAtive;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
