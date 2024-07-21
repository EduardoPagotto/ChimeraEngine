#pragma once
#include "Font.hpp"
#include <memory>
#include <unordered_map>

namespace Chimera {

// TODD: 1) usar shared_ptr<Font>, 2) colocar no ServiceLocate (mas TextureManager primeiro)

class FontManager {
    inline static std::unordered_map<std::string, std::shared_ptr<Font>> fonts;

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

    static std::shared_ptr<Font> get(const std::string& name) {
        auto got = fonts.find(name);
        if (got != fonts.end()) {
            return got->second;
        }

        return nullptr;
    }

    static std::shared_ptr<Font> get() { return fonts.begin()->second; }

    static std::shared_ptr<Font> getIndex(const uint16_t& index) {

        uint16_t count{0};
        for (auto it = fonts.begin(); it != fonts.end(); it++) {
            if (index == count)
                return it->second;

            count++;
        }

        return fonts.begin()->second;
    }

    static std::shared_ptr<Font> load(const std::string& name, const std::string& pathFile, const int& size) {
        auto font = std::make_shared<Font>(name, pathFile, size);
        fonts[name] = font;
        return font;
    }

  private:
    FontManager() {}
    ~FontManager() {}
};
} // namespace Chimera
