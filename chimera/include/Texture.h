#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "OpenGLDefs.h"
#include "Entity.h"

namespace Chimera {

class Texture : public Entity
{
public:
    Texture ( const std::string &_name, const std::string &_pathFile, const unsigned &_count );
	Texture ( const std::string &_name, const unsigned &_width, const unsigned &_height);
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

	//void createTextureFrameBuffer(const int & _width, const int & _height);

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

	unsigned count;

    std::string pathFile;

	unsigned width;
	unsigned height;

	GLuint idTexture;
   // int indiceFilter;
    bool texturaCarregada;
};
}
#endif
