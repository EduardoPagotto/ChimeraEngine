#include "chimera/core/visible/FontAtlas.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Chimera {

FontAtlas::FontAtlas(const std::string& name, const std::string& pathFile, const int& size)
    : texture(0), name(name), scale(glm::vec2(10.0f)) {

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

    const char* stylename = TTF_FontFaceStyleName(sFont);
    if (stylename)
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The name of the face in the font is: %s\n", stylename);

    const char* familyname = TTF_FontFaceFamilyName(sFont);
    if (familyname)
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "The family name of the face in the font is: %s\n", familyname);

    int val = TTF_GetFontKerning(sFont);

    SDL_Color fg = {0xff, 0xff, 0xff, 0xff};
    SDL_Color bg = {0x0, 0x0, 0x0, 0xff};

    uint16_t totW = 0;
    uint16_t maxH = 0;

    std::map<uint16_t, SDL_Surface*> mapGlyphCache;

    for (uint16_t c = 32; c < 127; c++) { // 0-256 ??
        // Load character glyph
        int minx, maxx, miny, maxy, advance;
        if (TTF_GlyphMetrics(sFont, c, &minx, &maxx, &miny, &maxy, &advance) == -1) {
            SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "TTF Erros: %s", TTF_GetError());
            continue;
        }

        // SDL_Surface* glyph_cache = TTF_RenderGlyph_Shaded(sFont, c, fg, bg);
        SDL_Surface* glyph_cache = TTF_RenderGlyph_Solid(sFont, c, fg);
        if (glyph_cache == nullptr)
            continue;

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
    }

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

    TexParam p;
    p.format = TexFormat::RGBA;
    p.wrap_r = TexWrap::CLAMP_TO_EDGE;
    p.wrap_s = TexWrap::CLAMP_TO_EDGE;
    p.wrap_t = TexWrap::CLAMP_TO_EDGE;
    p.minFilter = TexFilter::LINEAR;
    p.magFilter = TexFilter::LINEAR;
    texture = TextureManager::loadFromSurface(name, bigSurface, p);

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

FontAtlas::~FontAtlas() {
    // TODO: deletar glyph
    glyphs.clear();
}

} // namespace Chimera