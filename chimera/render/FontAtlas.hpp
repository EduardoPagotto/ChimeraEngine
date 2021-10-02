#ifndef __CHIMERA_FONT_ATLAS__HPP
#define __CHIMERA_FONT_ATLAS__HPP

#include "chimera/core/OpenGLDefs.hpp"
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
    inline const GLuint getTextureID() const { return texture; }
    inline void setScale(const glm::vec2& scale) { this->scale = scale; }
    inline glm::vec2 getScale() const { return this->scale; }

    std::map<uint16_t, GlyphData*> glyphs;

  private:
    int invert_image(int pitch, int height, void* image_pixels);
    glm::vec2 scale;
    GLuint texture;
    std::string name;
};

} // namespace Chimera

#endif