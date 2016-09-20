#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "OpenGLDefs.h"
#include "Entity.h"

namespace Chimera {

enum class TEX_SEQ {
	DIFFUSE = 0,
	SHADOWMAP = 1
};

class Texture : public Entity
{
public:
    Texture ( const std::string &_name, const TEX_SEQ &_indexTextureSeq, const std::string &_pathFile );
	Texture ( const std::string &_name, const TEX_SEQ &_indexTextureSeq, const unsigned &_width, const unsigned &_height );
    virtual ~Texture();

	void apply();

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

	TEX_SEQ getIndexTextureSeq() const {
		return indexTextureSeq;
	}

private:

	TEX_SEQ indexTextureSeq;

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
