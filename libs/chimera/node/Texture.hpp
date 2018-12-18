#ifndef __CHIMERA_TEXTURE__HPP
#define __CHIMERA_TEXTURE__HPP

#include "Entity.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Tex.hpp"

namespace Chimera {

class Texture : public Entity {
  public:
    Texture(const std::string& _name, const TEX_KIND& _indexTextureSeq, const std::string& _pathFile);
    virtual ~Texture();

    inline void apply(const std::string& _shaderPropName, Shader* _pShader) {
        pTex->apply((unsigned)indexTextureSeq, _shaderPropName, _pShader);
    }

    void init();

    TEX_KIND getIndexTextureSeq() const { return indexTextureSeq; }

  private:
    TEX_KIND indexTextureSeq;
    std::string pathFile;
    TexImg* pTex;
    bool texturaCarregada;
};
} // namespace Chimera
#endif
