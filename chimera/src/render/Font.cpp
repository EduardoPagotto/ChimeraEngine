#include "chimera/render/Font.hpp"
#include "chimera/core/Exception.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace Chimera {

Font::Font(const std::string& _fontFile, const int& _size) {

    // https://stackoverflow.com/questions/5289447/using-sdl-ttf-with-opengl
    // https://stackoverflow.com/questions/327642/opengl-and-monochrome-texture

    if (TTF_Init() == -1) {
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "TTF Erros: %s", TTF_GetError());
        return;
    }

    TTF_Font* sFont = TTF_OpenFont(_fontFile.c_str(), _size);

    int style;
    style = TTF_GetFontStyle(sFont);
    if (style == TTF_STYLE_NORMAL)
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The font style is: normal");
    else {
        if (style & TTF_STYLE_BOLD)
            SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The font style is: bold");
        if (style & TTF_STYLE_ITALIC)
            SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The font style is: italic");
        if (style & TTF_STYLE_UNDERLINE)
            SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The font style is: underline");
    }

    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The number of faces in the font is: %ld\n", TTF_FontFaces(sFont));

    char* stylename = TTF_FontFaceStyleName(sFont);
    if (stylename)
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The name of the face in the font is: %s\n", stylename);

    char* familyname = TTF_FontFaceFamilyName(sFont);
    if (familyname)
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The family name of the face in the font is: %s\n", familyname);

    SDL_Color fg = {0, 0, 0}, bg = {0xff, 0xff, 0xff};

    // Load first 128 characters of ASCII set
    for (uint16_t c = 1; c < 128; c++) {
        // Load character glyph

        int minx, maxx, miny, maxy, advance;
        if (TTF_GlyphMetrics(sFont, c, &minx, &maxx, &miny, &maxy, &advance) == -1) {
            SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "TTF Erros: %s", TTF_GetError());
            continue;
        }

        SDL_Surface* glyph_cache = TTF_RenderGlyph_Shaded(sFont, c, fg, bg);
        if (glyph_cache == nullptr)
            continue;

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph_cache->w, glyph_cache->h, 0, GL_RED, GL_UNSIGNED_BYTE, glyph_cache->pixels);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int botton = glyph_cache->h;

        // Now store character for later use
        Character character = {texture, glm::ivec2(glyph_cache->w, glyph_cache->h), glm::ivec2(minx, botton), (advance << 6)};

        Characters.insert(std::pair<uint16_t, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy font once we're finished

    if (sFont) {
        TTF_CloseFont(sFont);
    }

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
}

Font::~Font(void) {

    std::map<uint16_t, Character>::iterator it = Characters.begin();
    while (it != Characters.end()) {

        Character c = it->second;
        glDeleteTextures(1, (GLuint*)&c.TextureID);

        Characters.erase(it);
        it = Characters.begin();
    }
}

void Font::renderText(std::string* pText, const glm::ivec2& pos, GLfloat scale) {

    glBindVertexArray(VAO);

    float x = pos.x;
    float y = pos.y;

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = pText->begin(); c != pText->end(); c++) {
        Character ch = Characters[(uint16_t)(*c)];

        GLfloat right = x + ch.Bearing.x * scale;
        GLfloat left = right + (ch.Size.x * scale); // w
        GLfloat botton = y - (ch.Size.y - ch.Bearing.y) * scale;
        GLfloat top = botton + (ch.Size.y * scale); // h

        // Update VBO for each character
        GLfloat vertices[6][4] = {{right, top, 0.0, 0.0}, {right, botton, 0.0, 1.0}, {left, botton, 1.0, 1.0},
                                  {right, top, 0.0, 0.0}, {left, botton, 1.0, 1.0},  {left, top, 1.0, 0.0}};

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Be sure to use glBufferSubData and not glBufferData
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of
                                        // 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
}
} // namespace Chimera
