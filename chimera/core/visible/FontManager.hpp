#pragma once
#include "FontAtlas.hpp"
#include <vector>

namespace Chimera {

class FontManager {
  public:
    static void add(FontAtlas* font) { fonts.push_back(font); }

    static void clean() {
        for (int i = 0; i < fonts.size(); i++)
            delete fonts[i];

        fonts.clear();
    }
    static FontAtlas* get(const std::string& name) {
        // TODO: modificar mapara mapa

        for (FontAtlas* font : fonts) {
            if (font->getName() == name)
                return font;
        }

        // FIXME: colocar fonte default se elea nao carregar
        return nullptr;
    }

    static FontAtlas* get() { return fonts[0]; }

  private:
    FontManager() {}

    ~FontManager() {}

    inline static std::vector<FontAtlas*> fonts;
};
} // namespace Chimera
