#ifndef __CHIMERA_FONT_ATLAS__HPP
#define __CHIMERA_FONT_ATLAS__HPP

#include "chimera/OpenGLDefs.hpp"
#include <glm/glm.hpp>
#include <map>
#include <string>

namespace Chimera {

struct CharacterAtlas {
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    long int Advance;   // GLuint Advance;     // Horizontal offset to advance to next glyph
};

class FontAtlas {
  public:
    FontAtlas(const std::string& pathFile, const int& size);
    virtual ~FontAtlas();

  private:
    std::map<uint16_t, CharacterAtlas> Characters;
    GLuint texture;
};

} // namespace Chimera

#endif