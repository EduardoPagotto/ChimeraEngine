#pragma once

#include "chimera_core/gl/Font.hpp"
#include "chimera_core/gl/TextureLoader.hpp"
#include "chimera_core/gl/TextureParams.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <memory>

namespace ce {

    struct FontLoader {
        using result_type = std::shared_ptr<Font>;

        result_type operator()(const std::string& pathFile, const int& size) const {

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] File: %s, size: %d", pathFile.c_str(), size);

            // return FontLoader::LoadFromFile(registry, pathFile, size);
            if (!TTF_Init()) {
                throw std::runtime_error("[FontLoader] Falha ao inicial o TTF" + std::string(SDL_GetError()));
            }

            TTF_Font* sFont = TTF_OpenFont(pathFile.c_str(), static_cast<float>(size));

            uint32_t style = TTF_GetFontStyle(sFont);
            if (style == TTF_STYLE_NORMAL) {
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] Style: normal");

            } else {
                if ((style & TTF_STYLE_BOLD) != 0U) {
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] Style: bold");
                }

                if ((style & TTF_STYLE_ITALIC) != 0U) {
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] Style : italic");
                }

                if ((style & TTF_STYLE_UNDERLINE) != 0U) {
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] Style: underline");
                }
            }

            // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "The number of faces in the font is: %ld\n",
            // TTF_FontFaces(sFont));

            if (const char* stylename = TTF_GetFontStyleName(sFont); stylename) {
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] Face: %s\n", stylename);
            }

            if (const char* familyname = TTF_GetFontFamilyName(sFont); familyname) {
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] Family: %s\n", familyname);
            }

            // int val = TTF_GetFontKerning(sFont);
            const SDL_Color fg{0xff, 0xff, 0xff, 0xff};

            uint16_t totW{0};
            uint16_t maxH{0};

            std::map<uint16_t, SDL_Surface*> mapGlyphCache;
            std::map<uint16_t, Font::GlyphData> glyphs; // sera o final

            for (uint16_t c = 32; c < 127; c++) { // 0-256 ??
                // Load character glyph
                int minx;
                int maxx;
                int miny;
                int maxy;
                int advance;
                if (!TTF_GetGlyphMetrics(sFont, c, &minx, &maxx, &miny, &maxy, &advance)) {
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] TTF Erros: %s", SDL_GetError());
                    continue;
                }

                // SDL_Surface* glyph_cache = TTF_RenderGlyph_Shaded(sFont, c, fg, bg);
                SDL_Surface* glyph_cache = TTF_RenderGlyph_Solid(sFont, c, fg);
                if (glyph_cache == nullptr) {
                    continue;
                }

                totW += glyph_cache->w;

                maxH = std::max<int>(glyph_cache->h, maxH);

                // Now store character for later use
                Font::GlyphData lglyp; // = new Font::GlyphData;
                lglyp.size = glm::ivec2(glyph_cache->w, glyph_cache->h);
                lglyp.offset = glm::ivec2(minx, glyph_cache->h);
                lglyp.advance = advance;
                lglyp.square.x = 0.0F;
                lglyp.square.y = 0.0F;
                lglyp.square.w = 0.0F;
                lglyp.square.h = 0.0F;

                glyphs.insert(std::pair<uint16_t, Font::GlyphData>(c, lglyp));
                mapGlyphCache.insert(std::pair<uint16_t, SDL_Surface*>(c, glyph_cache));
            }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            Uint32 rmask = 0xff000000;
            Uint32 gmask = 0x00ff0000;
            Uint32 bmask = 0x0000ff00;
            Uint32 amask = 0x000000ff;
#else
            Uint32 rmask = 0x000000ff;
            Uint32 gmask = 0x0000ff00;
            Uint32 bmask = 0x00ff0000;
            Uint32 amask = 0xff000000;
#endif

            SDL_Surface* bigSurface =
                SDL_CreateSurface(totW, maxH, SDL_GetPixelFormatForMasks(32, rmask, gmask, bmask, amask));

            uint16_t nextX{0};
            SDL_Rect rect;
            for (const auto& kv : mapGlyphCache) {

                SDL_Surface* glyph_cache = kv.second;
                uint16_t characher = kv.first;

                rect.x = nextX;
                rect.y = 0.0;
                rect.w = nextX + glyph_cache->w;
                rect.h = glyph_cache->h;

                Font::GlyphData lglyp = glyphs[characher];
                lglyp.square.x = ((float)rect.x) / (float)totW;
                lglyp.square.y = 0.0;
                lglyp.square.w = ((float)rect.w) / (float)totW;
                lglyp.square.h = ((float)rect.h) / (float)maxH;

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

            // FIXME: precido disto abaixo ?
            // Invert_image_texture(bigSurface->pitch, bigSurface->h, bigSurface->pixels);
            std::shared_ptr<Texture> texture = TextureLoader::CreateFromSurface(bigSurface, p);

            if (sFont != nullptr) {
                TTF_CloseFont(sFont);
            }

            std::map<uint16_t, SDL_Surface*>::iterator it = mapGlyphCache.begin();
            while (it != mapGlyphCache.end()) {
                delete it->second;
                mapGlyphCache.erase(it);
                it = mapGlyphCache.begin();
            }

            // delete bigSurface;
            SDL_DestroySurface(bigSurface);

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[FontLoader] glyphs: %zu", glyphs.size());

            return std::make_shared<Font>(glyphs, texture);
        }
    };
} // namespace ce
