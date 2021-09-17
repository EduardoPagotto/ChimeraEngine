#ifndef __CHIMERA_FONT__HPP
#define __CHIMERA_FONT__HPP

#include "chimera/OpenGLDefs.hpp"
#include <glm/glm.hpp>
#include <map>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace Chimera {
/// Holds all state information relevant to a character as loaded using SDL_TTF
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    long int Advance;   // GLuint Advance;     // Horizontal offset to advance to next glyph
};

class Font {
  public:
    Font(const std::string& _fontFile, const int& _size);
    virtual ~Font(void);
    void renderText(std::string* pText, const glm::ivec2& pos, GLfloat scale);

  private:
    GLuint VAO, VBO;
    std::map<uint16_t, Character> Characters;
};
} // namespace Chimera
#endif
