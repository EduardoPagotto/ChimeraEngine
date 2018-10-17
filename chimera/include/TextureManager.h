#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <string>
#include <map>

#include <glm/glm.hpp>

#include "OpenGLDefs.h"
#include "Texture.h"

namespace Chimera {

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	unsigned int fromFile(std::string _name, const TEX_SEQ &_indexTextureSeq, std::string _pathFile);
	unsigned int fromFrameBuffer(std::string _name, const TEX_SEQ &_indexTextureSeq, const unsigned &_width, const unsigned &_height);

	void init(const unsigned int &_serial);
	void release(const unsigned int &_serial);
	void destroy(const unsigned int &_serial);
	void destroyAll();
	unsigned int getIdByName(const std::string &_name);

	glm::ivec2 getGeometria(const unsigned int &_serial);

	Texture *getTexture(const unsigned int &_serial);

 	void bind(const unsigned int &_serial);
 	static void unBindAll();

private:
	unsigned int addAvaible(Texture *_pTex);
	std::map<unsigned int, Texture*> mapTex;
};
}

#endif