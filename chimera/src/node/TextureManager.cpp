#include "chimera/node/TextureManager.hpp"

namespace Chimera {

TextureManager::TextureManager() noexcept {}

TextureManager::~TextureManager() {

    std::map<unsigned int, TexImg*>::iterator it = mapTex.begin();
    while (it != mapTex.end()) {

        TexImg* pTex = it->second;
        mapTex.erase(it);

        delete pTex;
        pTex = nullptr;

        it = mapTex.begin();
    }
}

TexImg* TextureManager::fromFile(const TEX_KIND& _indexTextureSeq, std::string _pathFile) {
    // TODO: colocar uma verificacao se o nome nao existe
    TexImg* tex = new TexImg(_indexTextureSeq, _pathFile);
    mapTex[tex->getSerial()] = tex;
    return tex;
}
} // namespace Chimera
