#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include<vector>

#include "Particle.h"

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

# include "Node.h"
# include "Texture.h"

namespace Chimera {
    
class ParticleEmitter : public Node
{
public:
    ParticleEmitter (Node *_parent, std::string _name );
    virtual ~ParticleEmitter();
    virtual void update ( DataMsg *_dataMsg );
	virtual void init();

	virtual void accept(class NodeVisitor* v);

    void initialize ( int _max );
    void loadImage ( const char *_file );
    void setGL();
    void render();
    void SortParticles ( const btVector3 &posCamera );

private:
    std::vector<Particle*> particles;
    std::vector<Color> coresPart;
    Texture *pTexture;
};
}
#endif
