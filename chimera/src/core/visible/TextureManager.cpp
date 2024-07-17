#include "chimera/core/visible/TextureManager.hpp"
#include <SDL2/SDL_image.h>

namespace Chimera {

void TextureManager::add(std::shared_ptr<Texture> texture) { textures.push_back(texture); }

void TextureManager::clean() {
    for (int i = 0; i < textures.size(); i++)
        textures[i] = nullptr;

    textures.clear();
}
void TextureManager::remove(const std::string& name) {

    for (int i = 0; i < textures.size(); i++) {
        if (textures[i]->getName() == name)
            textures[i] = nullptr;
    }
}

std::shared_ptr<Texture> TextureManager::get(const std::string& name) {
    // TODO: modificar mapara mapa
    for (std::shared_ptr<Texture> texture : textures) {
        if (texture->getName() == name)
            return texture;
    }

    // FIXME: colocar fonte default se elea nao carregar
    return nullptr;
}

void TextureManager::invert_image(int pitch, int height, void* image_pixels) {
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

std::shared_ptr<Texture> TextureManager::loadFromSurface(const std::string& name, SDL_Surface* surface, TexParam textureParameters) {

    TextureManager::invert_image(surface->pitch, surface->h, surface->pixels);

    std::shared_ptr<Texture> tex = std::make_shared<Texture>(name, surface, textureParameters);
    TextureManager::add(tex);

    return tex;
}

std::shared_ptr<Texture> TextureManager::loadFromFile(const std::string& name, const std::string& pathfile, TexParam textureParameters) {

    SDL_Surface* pImage = IMG_Load(pathfile.c_str());
    if (pImage == nullptr)
        throw std::string("Falha ao ler arquivo:" + pathfile);

    std::shared_ptr<Texture> tex = TextureManager::loadFromSurface(name, pImage, textureParameters);

    SDL_FreeSurface(pImage);

    return tex;
}

} // namespace Chimera