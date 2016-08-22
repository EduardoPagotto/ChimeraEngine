#ifndef FONT_H_
#define FONT_H_

//#define TTF_NOVO

// #ifdef TTF_NOVO
//
// #ifndef WIN32
// #include <SDL2/SDL_ttf.h>
// #else
// #include <SDL_ttf.h>
// #endif
//
// #else
// #include <FTGL/ftgl.h>
// #endif

//#include "ExceptionChimera.h"
#include <glm/glm.hpp>

#include "Color.h"
#include "OpenGLDefs.h"

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

#include "Shader.h"

namespace Chimera
{

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

class Font
{
public:
    Font ( const char*_fontFile, int _size );
    virtual ~Font ( void );

    //void render ( const float &_x, const float &_y, const float &_z, const Color &_color, std::string *_pTxt );

    void RenderText(Shader *pShader, std::string *pText, GLfloat x, GLfloat y, GLfloat scale, const Color &_color);

private:

    GLuint VAO, VBO;
    std::map<GLchar, Character> Characters;

// #ifdef TTF_NOVO
//     TTF_Font *pFont;
// #else
//     FTGLPixmapFont *pFont;
// #endif

};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
