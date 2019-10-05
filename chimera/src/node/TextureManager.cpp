#include "chimera/node/TextureManager.hpp"

namespace Chimera {

TextureManager::TextureManager() noexcept {}

TextureManager::~TextureManager() {

    std::map<unsigned int, Texture*>::iterator it = mapTex.begin();
    while (it != mapTex.end()) {

        Texture* pTex = it->second;
        mapTex.erase(it);

        delete pTex;
        pTex = nullptr;

        it = mapTex.begin();
    }
}

Texture* TextureManager::fromFile(std::string _name, const TEX_KIND& _indexTextureSeq, std::string _pathFile) {
    // TODO: colocar uma verificacao se o nome nao existe
    Texture* tex = new Texture(_name, _indexTextureSeq, _pathFile);
    mapTex[tex->getSerial()] = tex;
    return tex;
}
} // namespace Chimera
