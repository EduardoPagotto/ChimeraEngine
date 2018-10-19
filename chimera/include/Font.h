#ifndef FONT_H_
#define FONT_H_

#include <glm/glm.hpp>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Color.h"

#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include <OpenGLDefs.h>

namespace Chimera
{
/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    long int Advance;//GLuint Advance;     // Horizontal offset to advance to next glyph
};

class Font
{
public:
    Font ( const std::string &_fontFile,const int &_size );
    virtual ~Font ( void );
    void RenderText( std::string *pText, GLfloat x, GLfloat y, GLfloat scale);

private:
    GLuint VAO, VBO;
    std::map<GLchar, Character> Characters;
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
