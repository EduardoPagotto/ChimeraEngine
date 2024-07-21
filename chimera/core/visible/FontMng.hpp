#pragma once
#include "Font.hpp"
#include "chimera/core/ServiceLocator.hpp"
#include <unordered_map>

namespace Chimera {

class FontMng : public IService {
  private:
    std::unordered_map<std::string, std::shared_ptr<Font>> fonts;

  public:
    explicit FontMng() noexcept = default;
    FontMng(const FontMng& o) = delete;
    FontMng& operator=(const FontMng& o) = delete;
    virtual ~FontMng() noexcept { this->clear(); }

    std::type_index getTypeIndex() const { return std::type_index(typeid(FontMng)); }

    void clear() noexcept {
        for (auto it = fonts.begin(); it != fonts.end(); it++) {
            it->second = nullptr;
        }
        fonts.clear();
    }

    void remove(const std::string& name) noexcept {

        auto got = fonts.find(name);
        if (got != fonts.end()) {
            got->second = nullptr;
            fonts.erase(got);
        }
    }

    std::shared_ptr<Font> get(const std::string& name) noexcept {
        auto got = fonts.find(name);
        if (got != fonts.end()) {
            return got->second;
        }

        return nullptr;
    }

    std::shared_ptr<Font> get() noexcept { return fonts.begin()->second; }

    std::shared_ptr<Font> getIndex(const uint16_t& index) noexcept {

        uint16_t count{0};
        for (auto it = fonts.begin(); it != fonts.end(); it++) {
            if (index == count)
                return it->second;

            count++;
        }

        return fonts.begin()->second;
    }

    std::shared_ptr<Font> load(const std::string& name, const std::string& pathFile, const int& size) noexcept {
        auto font = std::make_shared<Font>(name, pathFile, size);
        fonts[name] = font;
        return font;
    }
};
} // namespace Chimera
