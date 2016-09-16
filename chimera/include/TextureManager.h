#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <string>
#include <map>

#include "OpenGLDefs.h"
#include "Texture.h"

namespace Chimera {

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	unsigned int fromFile(std::string _name, std::string _pathFile);
	unsigned int fromFrameBuffer(std::string _name, const unsigned &_width, const unsigned &_height);

	void init(const unsigned int &_serial);
	void addRef(const unsigned int &_serial);
	void releaseRef(const unsigned int &_serial);
	void destroy(const unsigned int &_serial);
	void destroyAll();
	unsigned int refCount(const unsigned int &_serial);
	unsigned int getIdByName(const std::string &_name);

	void bind(const unsigned int &_serial, unsigned int &_indice);
	void unBind();

private:

	int totalBind;

	unsigned int addAvaible(Texture *_pTex);
	std::map<unsigned int, Texture*> mapTex;
};
}

#endif
