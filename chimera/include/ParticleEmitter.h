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

namespace Chimera {

	class ParticleEmitter {
	public:

		ParticleEmitter();
		virtual ~ParticleEmitter();

		void initialize(int _max);

		void loadImage(const char *_file);

		void setGL();

		void render();

	private:
		GLuint texture[1];
		std::vector<Particle*> particles;
		std::vector<Color> coresPart;

	};
}
#endif
