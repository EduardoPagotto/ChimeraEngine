#ifndef __CHIMERA_TEXTURE__HPP
#define __CHIMERA_TEXTURE__HPP

#include "Entity.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/render/Tex.hpp"

namespace Chimera {

class Texture : public Entity {
  public:
    Texture(const std::string& _name, const TEX_KIND& _indexTextureSeq, const std::string& _pathFile);
    virtual ~Texture();

    inline void apply(Shader* _pShader) { pTex->apply(_pShader); }
    inline bool init() { return pTex->init(); }

    inline TexImg* inner() { return pTex; }

  private:
    // TEX_KIND indexTextureSeq;
    TexImg* pTex;
};
} // namespace Chimera
#endif
