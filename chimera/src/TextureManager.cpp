#include "TextureManager.h"


namespace Chimera {

TextureManager::TextureManager () {
}

TextureManager::~TextureManager() {

	destroyAll();
}

unsigned int TextureManager::fromFile(std::string _name, std::string _pathFile) {

	//TODO: colocar uma verificacao se o nome nao existe
	Texture *tex = new Texture(_name, _pathFile);
	return addAvaible(tex);

}

unsigned int TextureManager::fromFrameBuffer(std::string _name, const unsigned &_width, const unsigned &_height) {

	//TODO: colocar uma verificacao se o nome nao existe
	Texture *tex = new Texture(_name , _width, _height);
	return addAvaible(tex);

}

unsigned int TextureManager::addAvaible(Texture *_pTex) {

	mapTex[ _pTex->getSerial() ] = _pTex;
	return -_pTex->getSerial();
}

void TextureManager::init(const unsigned int &_serial) {

	Texture *pTex = mapTex[ _serial ];
	pTex->init();
	pTex->addRefCount();

}

unsigned int TextureManager::getIdByName(const std::string &_name) {

	unsigned int val = -1;
	for( std::map<unsigned int, Texture*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {

		val = iTex->first;
		Texture *pTex = iTex->second;

		if (pTex->getName().compare(_name) == 0)
			break;
	}

	return val;
}

void TextureManager::addRef(const unsigned int &_serial){
	Texture *pTex = mapTex[ _serial ];
	pTex->addRefCount();
}

void TextureManager::releaseRef(const unsigned int &_serial){
	Texture *pTex = mapTex[ _serial ];
	pTex->releaseRefCount();
}

unsigned int TextureManager::refCount(const unsigned int &_serial){
	Texture *pTex = mapTex[ _serial ];
	return pTex->getRefCount();
}

void TextureManager::destroy(const unsigned int &_serial) {

	std::map<unsigned int, Texture*>::iterator it=mapTex.find( _serial );

	Texture *pTex = it->second;
	mapTex.erase (it);

	delete pTex;
	pTex = nullptr;

}

void TextureManager::destroyAll() {

	std::map<unsigned int, Texture*>::iterator it = mapTex.begin();
	while (it != mapTex.end()) {

		Texture *pTex = it->second;
		mapTex.erase (it);

		delete pTex;
		pTex = nullptr;

		it = mapTex.begin();
	}
}

void TextureManager::bind(const unsigned int &_serial, unsigned int &_indice) {

	Texture *pTex = mapTex[ _serial ];
	pTex->apply(_indice);

}

void TextureManager::unBind() {

	glBindTexture(GL_TEXTURE_2D, 0);

}

}
