#ifndef __CHIMERA_TEXTURE_MANAGER__HPP
#define __CHIMERA_TEXTURE_MANAGER__HPP

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "Texture.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Singleton.hpp"

namespace Chimera {

class TextureManager {
    friend class Singleton<TextureManager>;

  public:
    Texture* fromFile(std::string _name, const TEX_KIND& _indexTextureSeq, std::string _pathFile);

  protected:
    TextureManager() noexcept;
    ~TextureManager();

  private:
    std::map<unsigned int, Texture*> mapTex;
};
} // namespace Chimera

#endif
