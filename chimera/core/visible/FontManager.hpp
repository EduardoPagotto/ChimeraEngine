#pragma once
#include "FontAtlas.hpp"
#include <unordered_map>

namespace Chimera {

// TODD: 1) usar shared_ptr<FontAtlas>, 2) colocar no ServiceLocate (mas TextureManager primeiro)

class FontManager {
  public:
    static void clean() {
        for (auto it = fonts.begin(); it != fonts.end(); it++) {
            it->second = nullptr;
        }
        fonts.clear();
    }

    static void remove(const std::string& name) {

        auto got = fonts.find(name);
        if (got != fonts.end()) {
            got->second = nullptr;
            fonts.erase(got);
        }
    }

    static FontAtlas* get(const std::string& name) {
        auto got = fonts.find(name);
        if (got != fonts.end()) {
            return got->second;
        }

        return nullptr;
    }

    static FontAtlas* get() { return fonts.begin()->second; }

    static FontAtlas* getIndex(const uint16_t& index) {

        uint16_t count{0};
        for (auto it = fonts.begin(); it != fonts.end(); it++) {
            if (index == count)
                return it->second;

            count++;
        }

        return fonts.begin()->second;
    }

    static FontAtlas* load(const std::string& name, const std::string& pathFile, const int& size) {
        FontAtlas* font = new FontAtlas(name, pathFile, size);
        fonts[name] = font;
        return font;
    }

  private:
    FontManager() {}
    ~FontManager() {}
    inline static std::unordered_map<std::string, FontAtlas*> fonts;
};
} // namespace Chimera
