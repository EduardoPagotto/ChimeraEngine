#ifdef WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include "chimera/core/Exception.hpp"
#include "chimera/node/Texture.hpp"

namespace Chimera {

Texture::Texture(const std::string& _name, const TEX_KIND& _indexTextureSeq, const std::string& _pathFile)
    : Entity(EntityKind::TEXTURE, _name) {
    indexTextureSeq = _indexTextureSeq;
    texturaCarregada = false;
    pathFile = _pathFile;
    pTex = new TexImg(_pathFile);
}

Texture::~Texture() { delete pTex; }

void Texture::init() {

    if (texturaCarregada == false) {
        pTex->init();
        texturaCarregada = true;
    }
}
} // namespace Chimera
