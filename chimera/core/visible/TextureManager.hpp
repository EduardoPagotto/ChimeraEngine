#pragma once
#include "Texture.hpp"
#include <SDL2/SDL_image.h>
#include <memory>
#include <unordered_map>

namespace Chimera {

class TextureManager {
  public:
    static void clean() noexcept {
        for (auto it = textures.begin(); it != textures.end(); it++) {
            it->second = nullptr;
        }

        textures.clear();
    }

    static void remove(const std::string& name) noexcept {
        auto got = textures.find(name);
        if (got != textures.end()) {
            got->second = nullptr;
            textures.erase(got);
        }
    }

    static std::shared_ptr<Texture> get(const std::string& name) noexcept {
        auto got = textures.find(name);
        if (got != textures.end()) {
            return got->second;
        }

        return std::shared_ptr<Texture>();
    }

    static std::shared_ptr<Texture> getIndex(const uint16_t& index) noexcept {
        uint16_t count{0};
        for (auto it = textures.begin(); it != textures.end(); it++) {
            if (index == count)
                return it->second;

            count++;
        }

        return textures.begin()->second;
    }

    static std::shared_ptr<Texture> loadFromSurface(const std::string& name, SDL_Surface* surface, TexParam textureParameters) noexcept {
        TextureManager::invert_image(surface->pitch, surface->h, surface->pixels);

        std::shared_ptr<Texture> tex = std::make_shared<Texture>(surface, textureParameters);
        textures[name] = tex;
        return tex;
    }

    static std::shared_ptr<Texture> loadFromFile(const std::string& name, const std::string& pathfile, TexParam textureParameters) {
        SDL_Surface* pImage = IMG_Load(pathfile.c_str());
        if (pImage == nullptr)
            throw std::string("Falha ao ler arquivo:" + pathfile);

        std::shared_ptr<Texture> tex = TextureManager::loadFromSurface(name, pImage, textureParameters);

        SDL_FreeSurface(pImage);

        return tex;
    }

  private:
    static void invert_image(int pitch, int height, void* image_pixels) {

        int index;
        void* temp_row;
        int height_div_2;

        temp_row = (void*)malloc(pitch);
        if (NULL == temp_row)
            throw std::string("Not enough memory for image inversion");

        // if height is odd, don't need to swap middle row
        height_div_2 = (int)(height * .5);
        for (index = 0; index < height_div_2; index++) {
            // uses string.h
            memcpy((Uint8*)temp_row, (Uint8*)(image_pixels) + pitch * index, pitch);

            memcpy((Uint8*)(image_pixels) + pitch * index, (Uint8*)(image_pixels) + pitch * (height - index - 1), pitch);
            memcpy((Uint8*)(image_pixels) + pitch * (height - index - 1), temp_row, pitch);
        }
        free(temp_row);
    }

    TextureManager() {}
    ~TextureManager() {}
    inline static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};
} // namespace Chimera
