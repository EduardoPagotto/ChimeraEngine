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

# include "Texture.h"
//# include "Node.h"

namespace Chimera {

	class ParticleEmitter : public Node {
	public:

		ParticleEmitter(std::string _id, std::string _name);
		virtual ~ParticleEmitter();

	    virtual void update(DataMsg *_dataMsg);

		void initialize(int _max);

		void loadImage(const char *_file);

		void setGL();

		void render();

	private:
		//GLuint texture[1];
		std::vector<Particle*> particles;
		std::vector<Color> coresPart;

		Texture *pTexture;

		//btVector3 source;

	};
}
#endif
