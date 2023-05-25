#include "chimera/core/visible/FontManager.hpp"

namespace Chimera {

void FontManager::add(FontAtlas* font) { fonts.push_back(font); }

FontAtlas* FontManager::get(const std::string& name) {

    // TODO: modificar mapara mapa

    for (FontAtlas* font : fonts) {
        if (font->getName() == name)
            return font;
    }

    // FIXME: colocar fonte default se elea nao carregar
    return nullptr;
}

void FontManager::clean() {

    for (int i = 0; i < fonts.size(); i++)
        delete fonts[i];

    fonts.clear();
}
} // namespace Chimera