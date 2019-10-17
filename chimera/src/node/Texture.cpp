#include <SDL2/SDL_image.h>

#include "chimera/core/Exception.hpp"
#include "chimera/node/Texture.hpp"

namespace Chimera {

Texture::Texture(const std::string& _name, const TEX_KIND& _indexTextureSeq, const std::string& _pathFile)
    : Entity(EntityKind::TEXTURE, _name) {
    pTex = new TexImg(_indexTextureSeq, _pathFile);
}

Texture::~Texture() { delete pTex; }

} // namespace Chimera
