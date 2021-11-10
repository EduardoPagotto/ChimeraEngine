#pragma once
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
    static Texture* getIndex(const uint16_t& index) { return textures[index]; }
    static bool loadFromSurface(const std::string& name, SDL_Surface* surface, TexParam textureParameters);
    static bool loadFromFile(const std::string& name, const std::string& pathfile, TexParam textureParameters);

  private:
    static int invert_image(int pitch, int height, void* image_pixels);
    TextureManager() {}
    ~TextureManager() {}
    static std::vector<Texture*> textures;
};
} // namespace Chimera