#pragma once
#include "chimera/render/OpenGLDefs.hpp"
#include "chimera/render/TextureManager.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

namespace Chimera {

struct GlyphData {
    glm::ivec2 size;   // Size of glyph
    glm::ivec2 offset; // Offset from baseline to left/top of glyph
    long int advance;  // GLuint Advance;     // Horizontal offset to advance to next glyph
    SDL_FRect square;
};

class FontAtlas {
  public:
    FontAtlas(const std::string& name, const std::string& pathFile, const int& size);
    virtual ~FontAtlas();
    inline const std::string getName() const { return name; }
    inline Texture* getTexture() const { return texture; }
    inline void setScale(const glm::vec2& scale) { this->scale = scale; }
    inline glm::vec2 getScale() const { return this->scale; }

    std::map<uint16_t, GlyphData*> glyphs;

  private:
    glm::vec2 scale;
    std::string name;
    Texture* texture;
};
} // namespace Chimera
