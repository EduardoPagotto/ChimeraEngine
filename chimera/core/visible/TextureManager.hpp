#pragma once
#include "Texture.hpp"
#include <memory>
#include <unordered_map>

namespace Chimera {

class TextureManager {
  public:
    static void clean();
    static void remove(const std::string& name);
    static std::shared_ptr<Texture> get(const std::string& name);
    static std::shared_ptr<Texture> getIndex(const uint16_t& index);
    static std::shared_ptr<Texture> loadFromSurface(const std::string& name, SDL_Surface* surface, TexParam textureParameters);
    static std::shared_ptr<Texture> loadFromFile(const std::string& name, const std::string& pathfile, TexParam textureParameters);

  private:
    static void invert_image(int pitch, int height, void* image_pixels);
    TextureManager() {}
    ~TextureManager() {}
    inline static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};
} // namespace Chimera
