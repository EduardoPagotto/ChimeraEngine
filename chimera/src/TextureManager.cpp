#include "TextureManager.hpp"

namespace Chimera {

TextureManager::TextureManager() noexcept {
    log = spdlog::get("chimera");
    log->debug("Constructor TextureManager");
}

TextureManager::~TextureManager() {
    destroyAll();
    log->debug("Destructor TextureManager");
}

unsigned int TextureManager::fromFile(std::string _name, const TEX_SEQ& _indexTextureSeq,
                                      std::string _pathFile) {

    // TODO: colocar uma verificacao se o nome nao existe
    Texture* tex = new Texture(_name, _indexTextureSeq, _pathFile);
    return addAvaible(tex);
}

unsigned int TextureManager::fromFrameBuffer(std::string _name,
                                             const TEX_SEQ& _indexTextureSeq,
                                             const unsigned& _width,
                                             const unsigned& _height) {

    // TODO: colocar uma verificacao se o nome nao existe
    Texture* tex = new Texture(_name, _indexTextureSeq, _width, _height);
    return addAvaible(tex);
}

unsigned int TextureManager::addAvaible(Texture* _pTex) {

    mapTex[_pTex->getSerial()] = _pTex;
    return _pTex->getSerial();
}

void TextureManager::init(const unsigned int& _serial) {

    Texture* pTex = this->getTexture(_serial);
    if (pTex != nullptr) {
        pTex->addRefCount();
        pTex->init();
    }
}

void TextureManager::release(const unsigned int& _serial) {

    std::map<unsigned int, Texture*>::iterator it = mapTex.find(_serial);
    if (it != mapTex.end()) {

        Texture* pTex = it->second;
        pTex->releaseRefCount();
        if (pTex->getRefCount() == 0) {

            mapTex.erase(it);

            delete pTex;
            pTex = nullptr;
        }
    }
}

unsigned int TextureManager::getIdByName(const std::string& _name) {

    unsigned int val = -1;
    for (std::map<unsigned int, Texture*>::iterator iTex = mapTex.begin();
         iTex != mapTex.end(); iTex++) {

        val = iTex->first;
        Texture* pTex = iTex->second;

        if (pTex->getName().compare(_name) == 0)
            break;
    }

    return val;
}

void TextureManager::destroy(const unsigned int& _serial) {

    std::map<unsigned int, Texture*>::iterator it = mapTex.find(_serial);

    Texture* pTex = it->second;
    mapTex.erase(it);

    delete pTex;
    pTex = nullptr;
}

void TextureManager::destroyAll() noexcept {

    std::map<unsigned int, Texture*>::iterator it = mapTex.begin();
    while (it != mapTex.end()) {

        Texture* pTex = it->second;
        mapTex.erase(it);

        delete pTex;
        pTex = nullptr;

        it = mapTex.begin();
    }
}

Texture* TextureManager::getTexture(const unsigned int& _serial) {

    Texture* pTex = nullptr;

    std::map<unsigned int, Texture*>::iterator it = mapTex.find(_serial);
    if (it != mapTex.end()) {
        pTex = it->second;
    } else {
        log->error("Erro, Textura nao existe id: {0:d}", _serial);
    }

    return pTex;
}

glm::ivec2 TextureManager::getGeometria(const unsigned int& _serial) {

    Texture* pTex = getTexture(_serial);
    return pTex != nullptr ? glm::ivec2(pTex->getWidth(), pTex->getHeight())
                           : glm::ivec2(640, 480);
}

void TextureManager::bind(const unsigned int& _serial) {

    Texture* pTex = getTexture(_serial);
    pTex->apply();
}

void TextureManager::unBindAll() { glBindTexture(GL_TEXTURE_2D, 0); }

} // namespace Chimera
