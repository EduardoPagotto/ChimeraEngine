#ifndef __CHIMERA_FONT__HPP
#define __CHIMERA_FONT__HPP

#include <glm/glm.hpp>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Color.hpp"

#include <OpenGLDefs.h>
#include <map>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace Chimera {
/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    long int Advance; // GLuint Advance;     // Horizontal offset to advance to next glyph
};

class Font {
  public:
    Font(const std::string& _fontFile, const int& _size);
    virtual ~Font(void);
    void RenderText(std::string* pText, GLfloat x, GLfloat y, GLfloat scale);

  private:
    GLuint VAO, VBO;
    std::map<GLchar, Character> Characters;
};
} // namespace Chimera
#endif
