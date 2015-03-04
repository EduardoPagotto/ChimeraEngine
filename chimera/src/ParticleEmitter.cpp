#include "ParticleEmitter.h"

#include "ExceptionChimera.h"


namespace Chimera {

	ParticleEmitter::ParticleEmitter() {

		coresPart.push_back(Color(1.0f, 0.5f, 0.5f));
		coresPart.push_back(Color(1.0f, 0.75f, 0.5f));
		coresPart.push_back(Color(1.0f, 1.0f, 0.5f));
		coresPart.push_back(Color(0.75f, 1.0f, 0.5f));
		coresPart.push_back(Color(0.5f, 1.0f, 0.5f));
		coresPart.push_back(Color(0.5f, 1.0f, 0.75f));
		coresPart.push_back(Color(0.5f, 1.0f, 1.0f));
		coresPart.push_back(Color(0.5f, 0.75f, 1.0f));
		coresPart.push_back(Color(0.75f, 0.5f, 1.0f));
		coresPart.push_back(Color(1.0f, 0.5f, 1.0f));
		coresPart.push_back(Color(1.0f, 0.5f, 0.75f));
	}

	ParticleEmitter::~ParticleEmitter() {

	}

	void ParticleEmitter::initialize(int _max) {

		particles.reserve(_max);

		/* Reset all the particles */
		for (int loop = 0; loop < _max; loop++)
		{
			int indiceCor = (loop + 1) / (_max / coresPart.size());

	        btVector3 novo((float)((rand() % 50) - 26.0f) * 10.0f,
							(float)((rand() % 50) - 25.0f) * 10.0f,
							(float)((rand() % 50) - 25.0f) * 10.0f  );           // yi e zi repetiam no original


			Particle *pParticle = new Particle();

	  		pParticle->ResetParticle( coresPart[indiceCor] , novo);

			particles.push_back(pParticle);
		}
	}

	void ParticleEmitter::setGL() {

		/* Enable smooth shading */
		glShadeModel(GL_SMOOTH);

		/* Set the background black */
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		/* Depth buffer setup */
		//glClearDepth(1.0f);

		/* Enables Depth Testing */
		glDisable(GL_DEPTH_TEST);

		/* Enable Blending */
		glEnable(GL_BLEND);

		/* Type Of Blending To Perform */
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		/* Really Nice Perspective Calculations */
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		/* Really Nice Point Smoothing */
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

		/* Enable Texture Mapping */
		glEnable(GL_TEXTURE_2D);

		/* Select Our Texture */
		glBindTexture(GL_TEXTURE_2D, texture[0]);

	}

	void ParticleEmitter::loadImage(const char *_file) {

		//SDL_Surface *pImage = IMG_Load(_file);
		//if (pImage == nullptr)
		//	throw ExceptionChimera(ExceptionCode::READ, "Falha ao ler arquivo:" + std::string(_file));
		/* Create storage space for the texture */
		SDL_Surface *TextureImage[1];

		/* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
		//if ((TextureImage[0] = SDL_LoadBMP("data/particle.bmp")))
		if ((TextureImage[0] = SDL_LoadBMP(_file)))
		{

			/* Set the status to true */
			//Status = TRUE;

			/* Create The Texture */
			glGenTextures(1, &texture[0]);

			/* Typical Texture Generation Using Data From The Bitmap */
			glBindTexture(GL_TEXTURE_2D, texture[0]);

			/* Generate The Texture */
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->w,
				TextureImage[0]->h, 0, GL_BGR_EXT,
				GL_UNSIGNED_BYTE, TextureImage[0]->pixels);

			/* Linear Filtering */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			throw ExceptionChimera(ExceptionCode::READ, "Falha ao ler arquivo:" + std::string(_file));
		}

		/* Free up any memory we may have used */
		if (TextureImage[0])
			SDL_FreeSurface(TextureImage[0]);

	}

	void ParticleEmitter::render() {

		for (unsigned loop = 0; loop < particles.size() ; loop++)
		{
			Particle *pParticle = particles[loop];
			pParticle->render();
		}

	}

}