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

    log = spdlog::get("chimera");
    log->debug("Constructor textura nome:{} arquivo:{}", _name, _pathFile);
}

Texture::~Texture() { delete pTex; }

void Texture::apply(const std::string& _shaderPropName, Shader* _pShader) {

    // glActiveTexture(GL_TEXTURE0 + (unsigned)indexTextureSeq);
    // glBindTexture(GL_TEXTURE_2D, idTexture);
    pTex->apply((unsigned)indexTextureSeq, _shaderPropName, _pShader);
}

void Texture::init() {

    if (texturaCarregada == false) {
        pTex->init();
        texturaCarregada = true;
        log->info("Instanciada textura Nome:{0}", getName());
    }
}
} // namespace Chimera
