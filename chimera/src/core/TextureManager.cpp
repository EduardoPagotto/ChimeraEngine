#include "chimera/core/TextureManager.hpp"

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

void TextureManager::initAll() {
    for (Texture* texture : textures) {
        texture->init();
    }
}

} // namespace Chimera