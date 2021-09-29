#ifndef __CHIMERA_FONT_MANAGER__HPP
#define __CHIMERA_FONT_MANAGER__HPP

#include "FontAtlas.hpp"
#include <vector>

namespace Chimera {

class FontManager {
  public:
    static void add(FontAtlas* font);
    static void clean();
    static FontAtlas* get(const std::string& name);
    static FontAtlas* get() { return fonts[0]; }

  private:
    FontManager() {}
    ~FontManager() {}
    static std::vector<FontAtlas*> fonts;
};

} // namespace Chimera

#endif