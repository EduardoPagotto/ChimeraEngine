#ifndef __CHIMERA_TEXTURE_MANAGER__HPP
#define __CHIMERA_TEXTURE_MANAGER__HPP

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "Texture.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Singleton.hpp"

namespace Chimera {

class TextureManager {
    friend class Singleton<TextureManager>;

  public:
    Texture* fromFile(std::string _name, const TEX_SEQ& _indexTextureSeq, std::string _pathFile);

    // void init(const unsigned int& _serial);
    void release(const unsigned int& _serial);
    void destroy(const unsigned int& _serial);
    void destroyAll() noexcept;

  protected:
    TextureManager() noexcept;
    ~TextureManager();

  private:
    std::map<unsigned int, Texture*> mapTex;
    std::shared_ptr<spdlog::logger> log;
};
} // namespace Chimera

#endif
