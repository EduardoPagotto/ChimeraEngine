#pragma once
#include "Texture.hpp"
#include <memory>
#include <vector>

namespace Chimera {

class TextureManager {
  public:
    static void add(std::shared_ptr<Texture> texture);
    static void clean();
    static void remove(const std::string& name);
    static std::shared_ptr<Texture> get(const std::string& name);
    static std::shared_ptr<Texture> getIndex(const uint16_t& index) { return textures[index]; }
    static std::shared_ptr<Texture> loadFromSurface(const std::string& name, SDL_Surface* surface, TexParam textureParameters);
    static std::shared_ptr<Texture> loadFromFile(const std::string& name, const std::string& pathfile, TexParam textureParameters);

  private:
    static void invert_image(int pitch, int height, void* image_pixels);
    TextureManager() {}
    ~TextureManager() {}
    inline static std::vector<std::shared_ptr<Texture>> textures;
};
} // namespace Chimera
