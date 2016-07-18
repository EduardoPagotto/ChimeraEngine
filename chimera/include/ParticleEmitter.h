#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include<vector>

#include "Particle.h"

// #ifndef WIN32
// #include <SDL2/SDL.h>
// #else
// #include <SDL.h>
// #include "windows.h"
// #endif
// 
// #include <GL/gl.h>
// #include <GL/glu.h>

# include "Draw.h"
# include "Texture.h"

namespace Chimera {
    
class ParticleEmitter : public Draw
{
public:
    ParticleEmitter (Node *_parent, std::string _name );
    virtual ~ParticleEmitter();
    virtual void update ( DataMsg *_dataMsg );
	virtual void init();
	virtual void accept(class NodeVisitor* v);
    virtual void renderExecute(bool _texture);
    virtual btVector3 getSizeBox();
    
    void setGL();
    void loadImage ( const char *_file );
    void SortParticles ( const btVector3 &posCamera );
    void setSizeBox(const btVector3 &_size);

	inline void setPosSource(const btVector3 &_val) {
		posSource = _val;
	}

	inline btVector3 getPosSource() const {
		return posSource;
	}

	inline void setMaxSeed(const int &_max) {
		maxSeed = _max;
	}

	inline int getMaxSeed() const {
		return maxSeed;
	}

private:
    
	btVector3 posSource;
    btVector3 sizeBox;
    
	int maxSeed;
    
    std::vector<Particle*> particles;
    std::vector<Color> coresPart;
    //Texture *pTexture;
};
}
#endif
