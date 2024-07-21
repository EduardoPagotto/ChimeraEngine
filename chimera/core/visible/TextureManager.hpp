#pragma once
#include "Texture.hpp"
#include "chimera/core/ServiceLocator.hpp"
#include <SDL2/SDL_image.h>
#include <memory>
#include <unordered_map>

namespace Chimera {

class TextureManager : public IService {
  private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

  public:
    TextureManager() noexcept {}
    TextureManager(const TextureManager& o) = delete;
    TextureManager& operator=(const TextureManager& o) = delete;
    virtual ~TextureManager() noexcept { this->clean(); }
    std::type_index getTypeIndex() const { return std::type_index(typeid(TextureManager)); }

    void clean() noexcept {
        for (auto it = textures.begin(); it != textures.end(); it++) {
            it->second = nullptr;
        }

        textures.clear();
    }

    void remove(const std::string& name) noexcept {
        auto got = textures.find(name);
        if (got != textures.end()) {
            got->second = nullptr;
            textures.erase(got);
        }
    }

    std::shared_ptr<Texture> get(const std::string& name) noexcept {
        auto got = textures.find(name);
        if (got != textures.end()) {
            return got->second;
        }

        return std::shared_ptr<Texture>();
    }

    std::shared_ptr<Texture> getIndex(const uint16_t& index) noexcept {
        uint16_t count{0};
        for (auto it = textures.begin(); it != textures.end(); it++) {
            if (index == count)
                return it->second;

            count++;
        }

        return textures.begin()->second;
    }

    std::shared_ptr<Texture> loadFromSurface(const std::string& name, SDL_Surface* surface, TexParam textureParameters) noexcept {
        Texture::invert_image_texture(surface->pitch, surface->h, surface->pixels);

        std::shared_ptr<Texture> tex = std::make_shared<Texture>(surface, textureParameters);
        textures[name] = tex;
        return tex;
    }

    std::shared_ptr<Texture> loadFromFile(const std::string& name, const std::string& pathfile, TexParam textureParameters) {
        SDL_Surface* pImage = IMG_Load(pathfile.c_str());
        if (pImage == nullptr)
            throw std::string("Falha ao ler arquivo:" + pathfile);

        std::shared_ptr<Texture> tex = this->loadFromSurface(name, pImage, textureParameters);

        SDL_FreeSurface(pImage);

        return tex;
    }
};
} // namespace Chimera
