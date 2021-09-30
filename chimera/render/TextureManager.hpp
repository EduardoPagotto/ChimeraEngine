#ifndef __CHIMERA_TEXTURE_MANAGER_21__HPP
#define __CHIMERA_TEXTURE_MANAGER_21__HPP

#include "Texture.hpp"
#include <vector>

namespace Chimera {

class TextureManager {
  public:
    static void add(Texture* texture);
    static void clean();
    static void remove(const std::string& name);
    static Texture* get(const std::string& name);
    static Texture* get() { return textures[0]; }
    static Texture* getLast() { return textures.back(); }

  private:
    TextureManager() {}
    ~TextureManager() {}
    static std::vector<Texture*> textures;
};

} // namespace Chimera

#endif
