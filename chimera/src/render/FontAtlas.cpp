#include "chimera/render/FontAtlas.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Chimera {

FontAtlas::FontAtlas(const std::string& name, const std::string& pathFile, const int& size) : texture(0), name(name) {

    if (TTF_Init() == -1) {
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "TTF Erros: %s", TTF_GetError());
        return;
    }

    TTF_Font* sFont = TTF_OpenFont(pathFile.c_str(), size);

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

    int val = TTF_GetFontKerning(sFont);

    SDL_Color fg = {0xff, 0xff, 0xff, 0xff};
    SDL_Color bg = {0x0, 0x0, 0x0, 0xff};

    uint16_t totW = 0;
    uint16_t maxH = 0;

    std::map<uint16_t, SDL_Surface*> mapGlyphCache;

    // Load first 128 characters of ASCII set
    // for (uint16_t c = 32; c < 127; c++) { // 0-256 ??
    // Load character glyph

    uint16_t c = 64;

    int minx, maxx, miny, maxy, advance;
    if (TTF_GlyphMetrics(sFont, c, &minx, &maxx, &miny, &maxy, &advance) == -1) {
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "TTF Erros: %s", TTF_GetError());
        // continue;
    }

    // SDL_Surface* glyph_cache = TTF_RenderGlyph_Shaded(sFont, c, fg, bg);
    SDL_Surface* glyph_cache = TTF_RenderGlyph_Solid(sFont, c, fg);
    // if (glyph_cache == nullptr)
    //     continue;

    totW += glyph_cache->w;

    if (glyph_cache->h > maxH)
        maxH = glyph_cache->h;

    // Now store character for later use
    GlyphData* pGlyp = new GlyphData;
    pGlyp->size = glm::ivec2(glyph_cache->w, glyph_cache->h);
    pGlyp->offset = glm::ivec2(minx, glyph_cache->h);
    pGlyp->advance = advance;
    pGlyp->square.x = 0.0f;
    pGlyp->square.y = 0.0f;
    pGlyp->square.w = 0.0f;
    pGlyp->square.h = 0.0f;

    glyphs.insert(std::pair<uint16_t, GlyphData*>(c, pGlyp));
    mapGlyphCache.insert(std::pair<uint16_t, SDL_Surface*>(c, glyph_cache));
    //}

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface* bigSurface = SDL_CreateRGBSurface(0, totW, maxH, 32, rmask, gmask, bmask, amask);

    uint16_t nextX = 0;
    SDL_Rect rect;
    for (const auto& kv : mapGlyphCache) {

        SDL_Surface* glyph_cache = kv.second;
        uint16_t characher = kv.first;

        rect.x = nextX;
        rect.y = 0.0;
        rect.w = nextX + glyph_cache->w;
        rect.h = glyph_cache->h;

        GlyphData* pGlyp = glyphs[characher];
        pGlyp->square.x = ((float)rect.x) / (float)totW;
        pGlyp->square.y = 0.0;
        pGlyp->square.w = ((float)rect.w) / (float)totW;
        pGlyp->square.h = ((float)rect.h) / (float)maxH;

        SDL_BlitSurface(glyph_cache, nullptr, bigSurface, &rect);

        // char buff[50];
        // snprintf(buff, sizeof(buff), (const char*)"./tst/caracter_%d.png", characher);
        // SDL_SaveBMP(glyph_cache, buff);

        nextX += glyph_cache->w;
    }

    // SDL_SaveBMP(bigSurface, "./tst/atlas.png");
    if (invert_image(bigSurface->pitch, bigSurface->h, bigSurface->pixels) != 0) {
        SDL_SetError("Falha na inversao de pixels");
    }

    // Criar Textura
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (bigSurface->format->Amask != 0) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bigSurface->w, bigSurface->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, bigSurface->pixels);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bigSurface->w, bigSurface->h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bigSurface->pixels);
    }

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (sFont) {
        TTF_CloseFont(sFont);
    }

    std::map<uint16_t, SDL_Surface*>::iterator it = mapGlyphCache.begin();
    while (it != mapGlyphCache.end()) {
        delete it->second;
        mapGlyphCache.erase(it);
        it = mapGlyphCache.begin();
    }

    delete bigSurface;
}

int FontAtlas::invert_image(int pitch, int height, void* image_pixels) {
    int index;
    void* temp_row;
    int height_div_2;

    temp_row = (void*)malloc(pitch);
    if (NULL == temp_row) {
        SDL_SetError("Not enough memory for image inversion");
        return -1;
    }
    // if height is odd, don't need to swap middle row
    height_div_2 = (int)(height * .5);
    for (index = 0; index < height_div_2; index++) {
        // uses string.h
        memcpy((Uint8*)temp_row, (Uint8*)(image_pixels) + pitch * index, pitch);

        memcpy((Uint8*)(image_pixels) + pitch * index, (Uint8*)(image_pixels) + pitch * (height - index - 1), pitch);
        memcpy((Uint8*)(image_pixels) + pitch * (height - index - 1), temp_row, pitch);
    }
    free(temp_row);
    return 0;
}

FontAtlas::~FontAtlas() {
    // TODO: deletar glyph
    glyphs.clear();
}

} // namespace Chimera