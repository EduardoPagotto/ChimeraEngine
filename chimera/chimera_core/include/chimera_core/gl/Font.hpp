#pragma once
#include "chimera_core/gl/Texture.hpp"
#include <SDL3/SDL_rect.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>

namespace ce {

    struct Font {

        struct GlyphData {
            glm::ivec2 size;   // Size of glyph
            glm::ivec2 offset; // Offset from baseline to left/top of glyph
            long int advance;  // GLuint Advance;     // Horizontal offset to advance to next glyph
            SDL_FRect square;
        };

        explicit Font(const std::map<uint16_t, GlyphData>& glyphs, std::shared_ptr<Texture> texture) {
            this->glyphs = std::move(glyphs);
            this->texture = texture;
            this->scale = glm::vec2{1.0F};
        }

        virtual ~Font() = default;

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        std::map<uint16_t, GlyphData> glyphs; // TODO: ver com integrar no Draw2d
        glm::vec2 scale;
        std::shared_ptr<Texture> texture;
    };
} // namespace ce
