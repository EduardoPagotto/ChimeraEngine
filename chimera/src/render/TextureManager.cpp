#include "chimera/render/TextureManager.hpp"
#include "chimera/render/Exception.hpp"
#include <SDL2/SDL_image.h>

namespace Chimera {

std::vector<Texture*> TextureManager::textures;

void TextureManager::add(Texture* texture) { textures.push_back(texture); }

void TextureManager::clean() {
    for (int i = 0; i < textures.size(); i++)
        delete textures[i];

    textures.clear();
}
void TextureManager::remove(const std::string& name) {

    for (int i = 0; i < textures.size(); i++) {
        if (textures[i]->getName() == name)
            delete textures[i];
    }
}

Texture* TextureManager::get(const std::string& name) {
    // TODO: modificar mapara mapa
    for (Texture* texture : textures) {
        if (texture->getName() == name)
            return texture;
    }

    // FIXME: colocar fonte default se elea nao carregar
    return nullptr;
}

int TextureManager::invert_image(int pitch, int height, void* image_pixels) {
    int index;
    void* temp_row;
    int height_div_2;

    temp_row = (void*)malloc(pitch);
    if (NULL == temp_row) {
        SDL_SetError("Not enough memory for image inversion");
        return -1;
    }
    // if height is odd, don't need to swap middle row
    height_div_2 = (int)(height * .5);
    for (index = 0; index < height_div_2; index++) {
        // uses string.h
        memcpy((Uint8*)temp_row, (Uint8*)(image_pixels) + pitch * index, pitch);

        memcpy((Uint8*)(image_pixels) + pitch * index, (Uint8*)(image_pixels) + pitch * (height - index - 1), pitch);
        memcpy((Uint8*)(image_pixels) + pitch * (height - index - 1), temp_row, pitch);
    }
    free(temp_row);
    return 0;
}

bool TextureManager::loadFromSurface(const std::string& name, SDL_Surface* surface, TexParam textureParameters) {

    if (TextureManager::invert_image(surface->pitch, surface->h, surface->pixels) != 0) {
        SDL_SetError("Falha na inversao de pixels");
    }

    TextureManager::add(new Texture(name, surface, textureParameters));

    return true;
}

bool TextureManager::loadFromFile(const std::string& name, const std::string& pathfile, TexParam textureParameters) {

    SDL_Surface* pImage = IMG_Load(pathfile.c_str());
    if (pImage == nullptr)
        throw Exception("Falha ao ler arquivo:" + pathfile);

    bool res = TextureManager::loadFromSurface(name, pImage, textureParameters);

    SDL_FreeSurface(pImage);

    return res;
}

} // namespace Chimera