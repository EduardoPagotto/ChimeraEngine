#include "Font.h"

#include <ExceptionSDL.h>
//
// #ifdef WIN32
// #include "windows.h"
// #endif
// #include <GL/gl.h>


#include <iostream>



namespace Chimera {



Font::Font ( const char* _fontFile, int _size ) {

    // FreeType
    FT_Library ft;

    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        throw ExceptionSDL ( ExceptionCode::ALLOC, "Falha ao iniciar o FreeType" );

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, _fontFile, 0, &face))
        throw ExceptionSDL ( ExceptionCode::READ, "Arquivo de fonte falha ao carregar:" + std::string ( _fontFile ) );

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, _size);


    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

// #ifdef TTF_NOVO
//
//     pFont = TTF_OpenFont ( _fontFile, _size );
//     if ( !pFont ) {
//         throw ExceptionSDL ( ExceptionCode::READ, "Arquivo de fonte falha ao carregar:" + std::string ( _fontFile ) );
//     }
//
//     TTF_SetFontStyle ( pFont, TTF_STYLE_NORMAL );
//
// #else
//     if ( _fontFile == nullptr ) {
//         ExceptionChimera ( ExceptionCode::READ, "Arquivo de Fonte Nulo" );
//     }
//
//     pFont = new FTGLPixmapFont ( _fontFile );
//     if ( pFont == nullptr ) {
//         ExceptionChimera ( ExceptionCode::READ, "Carga de arquivo invalida" );
//     }
//
//     if ( pFont->Error() ) {
//         ExceptionChimera ( ExceptionCode::READ, "Carga de Fonte invalida" );
//     }
//
//     if ( pFont->FaceSize ( _size ) == false ) {
//         if ( pFont ) {
//             delete pFont;
//             pFont = nullptr;
//         }
//         ExceptionChimera ( ExceptionCode::READ, "Tamanho Fonte invalida" );
//     }
//
// #endif
}

Font::~Font ( void ) {

// #ifdef TTF_NOVO
//     TTF_CloseFont ( pFont );
//     pFont = nullptr;
// #else
//
//     if ( pFont != nullptr ) {
//         delete pFont;
//         pFont = nullptr;
//     }
// #endif

}

void Font::RenderText(Shader *pShader, std::string *pText, GLfloat x, GLfloat y, GLfloat scale, const Color &_color)
{
    // Activate corresponding render state
    pShader->link();//shader.Use();
    glUniform3f(glGetUniformLocation(pShader->getIdProgram(), "textColor"), _color.r, _color.g, _color.b);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = pText->begin(); c != pText->end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// void Font::render ( const float &_x, const float &_y, const float &_z, const Color &_color, std::string *_pTxt ) {
//
// // #ifdef TTF_NOVO
// //     SDL_Color Color = { ( uint8_t ) ( 255 * _color.r ), ( uint8_t ) ( 255 * _color.g ), ( uint8_t ) ( 255 * _color.b ) };
// //     SDL_Surface *Message = TTF_RenderText_Blended ( const_cast<TTF_Font*> ( pFont ), _pTxt->c_str(), Color );
// //     unsigned Texture = 0;
// //
// //     /*Generate an OpenGL 2D texture from the SDL_Surface*.*/
// //     glGenTextures ( 1, &Texture );
// //     glBindTexture ( GL_TEXTURE_2D, Texture );
// //
// //     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
// //     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
// //
// //     glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA_EXT,
// //                    GL_UNSIGNED_BYTE, Message->pixels );
// //
// //     /*Draw this texture on a quad with the given xyz coordinates.*/
// //     glBegin ( GL_QUADS );
// //     glTexCoord2d ( 0, 1 );
// //     glVertex3d ( _x, _y, _z );
// //     glTexCoord2d ( 1, 1 );
// //     glVertex3d ( _x + Message->w, _y, _z );
// //     glTexCoord2d ( 1, 0 );
// //     glVertex3d ( _x + Message->w, _y + Message->h, _z );
// //     glTexCoord2d ( 0, 0 );
// //     glVertex3d ( _x, _y + Message->h, _z );
// //     glEnd();
// //
// //     /*Clean up.*/
// //     glDeleteTextures ( 1, &Texture );
// //     SDL_FreeSurface ( Message );
// // #else
// //
// //     glColor3fv ( _color.ptr() );
// //     glRasterPos2f ( _x, _y );
// //     pFont->Render ( _pTxt->c_str(), _pTxt->size() );
// //
// // #endif
// }

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
