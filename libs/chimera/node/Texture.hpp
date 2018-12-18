#ifndef __CHIMERA_TEXTURE__HPP
#define __CHIMERA_TEXTURE__HPP

#include "Entity.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Tex.hpp"

#include <spdlog/spdlog.h>

namespace Chimera {

class Texture : public Entity {
  public:
    Texture(const std::string& _name, const TEX_KIND& _indexTextureSeq, const std::string& _pathFile);
    virtual ~Texture();

    void apply(const std::string& _shaderPropName, Shader* _pShader);

    void init();

    TEX_KIND getIndexTextureSeq() const { return indexTextureSeq; }

  private:
    TEX_KIND indexTextureSeq;
    std::string pathFile;

    TexImg* pTex;

    bool texturaCarregada;
    std::shared_ptr<spdlog::logger> log;
};
} // namespace Chimera
#endif
