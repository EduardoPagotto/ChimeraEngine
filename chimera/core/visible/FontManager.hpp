#pragma once
#include "FontAtlas.hpp"
#include <vector>

namespace Chimera {

class FontManager {
  public:
    static FontAtlas* load(const std::string& name, const std::string& pathFile, const int& size) {
        FontAtlas* font = new FontAtlas(name, pathFile, size);
        fonts.push_back(font);
        return font;
    }

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
