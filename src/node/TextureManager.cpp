#include "chimera/node/TextureManager.hpp"

namespace Chimera {

TextureManager::TextureManager() noexcept {
    log = spdlog::get("chimera");
    log->debug("Constructor TextureManager");
}

TextureManager::~TextureManager() {
    destroyAll();
    log->debug("Destructor TextureManager");
}

Texture* TextureManager::fromFile(std::string _name, const TEX_SEQ& _indexTextureSeq, std::string _pathFile) {
    // TODO: colocar uma verificacao se o nome nao existe
    Texture* tex = new Texture(_name, _indexTextureSeq, _pathFile);
    mapTex[tex->getSerial()] = tex;
    return tex;
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

} // namespace Chimera
