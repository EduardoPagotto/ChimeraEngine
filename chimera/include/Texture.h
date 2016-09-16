#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "OpenGLDefs.h"
#include "Entity.h"

namespace Chimera {

class Texture : public Entity
{
public:
    Texture ( const std::string &_name, const std::string &_pathFile );
	Texture ( const std::string &_name, const unsigned &_width, const unsigned &_height);
    Texture ( const Texture &_texture );

    virtual ~Texture();

	void apply(const unsigned int &_active);

    void init();

    bool textureLoad() {
        return texturaCarregada;
    }

	int getIdTexture() {
		return idTexture;
	}

	unsigned getWidth() const {
		return width;
	}

	unsigned getHeight() const {
		return height;
	}

	unsigned int getRefCount() const {
		return refCount;
	}

	void addRefCount() {
		refCount++;
	}

	void releaseRefCount() {
		if (refCount > 0)
			refCount--;
	}

	GLuint getFrameBufferId() const {
		return depthMapFBO;
	}

private:
	GLuint depthMapFBO;

	unsigned int refCount;

    std::string pathFile;

	unsigned width;
	unsigned height;

	GLuint idTexture;
   // int indiceFilter;
    bool texturaCarregada;
};
}
#endif
