#ifndef TEXTURE_H_
#define TEXTURE_H_

#ifdef WIN32
#include "SDL_image.h"
#else
#include "SDL2/SDL_image.h"
#endif
#include "ExceptionChimera.h"

#include "OpenGLDefs.h"

#include "Entity.h"

namespace Chimera {

class Texture : public Entity
{
public:
    Texture ( std::string _name, std::string _pathFile, unsigned _count );
    Texture ( const Texture &_texture );

    virtual ~Texture();

	void apply();

    void init();

    bool textureLoad() {
        return texturaCarregada;
    }

	int getIdTexture() {
		return idTexture;
	}

	unsigned getCount() const {
		return count;
	}

	GLuint createTextureFrameBuffer(const int & _width, const int & _height);

	unsigned getWidth() const {
		return width;
	}

	unsigned getHeight() const {
		return height;
	}

private:

	unsigned count;

	SDL_Surface *loadImage();

    std::string pathFile;

	unsigned width;
	unsigned height;

	GLuint idTexture;
    int indiceFilter;
    bool texturaCarregada;
};
}
#endif
