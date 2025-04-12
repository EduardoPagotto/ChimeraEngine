#pragma once
#include "TextureMng.hpp"
#include <glm/glm.hpp>
#include <map>
#include <string>

namespace ce {

struct GlyphData {
    glm::ivec2 size;   // Size of glyph
    glm::ivec2 offset; // Offset from baseline to left/top of glyph
    long int advance;  // GLuint Advance;     // Horizontal offset to advance to next glyph
    SDL_FRect square;
};

class Font {
  public:
    Font(const std::string& pathFile, const int& size);
    virtual ~Font();
    inline std::shared_ptr<Texture> getTexture() const { return texture; }
    inline void setScale(const glm::vec2& scale) { this->scale = scale; }
    inline glm::vec2 getScale() const { return this->scale; }

    std::map<uint16_t, GlyphData*> glyphs; // TODO: ver com integrar no Draw2d

  private:
    glm::vec2 scale;
    std::shared_ptr<Texture> texture;
};
} // namespace ce
