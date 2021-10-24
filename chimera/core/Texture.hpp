#ifndef __CHIMERA_TEX__HPP
#define __CHIMERA_TEX__HPP

#include "chimera/core/Shader.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

enum class TextureWrap {
    NONE = 0,
    REPEAT = GL_REPEAT,
    CLAMP = GL_CLAMP,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};

enum class TextureFilter { NONE = 0, LINEAR = GL_LINEAR, NEAREST = GL_NEAREST };

enum class TextureFormat {
    NONE = 0,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    LUMINANCE = GL_LUMINANCE,
    LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT
};

enum class TextureDataType { UNSIGNED_BYTE = GL_UNSIGNED_BYTE, UNSIGNED_SHORT = GL_UNSIGNED_SHORT, FLOAT = GL_FLOAT };

struct TextureParameters {
    TextureFormat format;
    TextureFilter filter;
    TextureWrap wrap;
    TextureDataType type;

    TextureParameters() {
        format = TextureFormat::RGBA;
        filter = TextureFilter::NEAREST;
        wrap = TextureWrap::REPEAT;
        type = TextureDataType::UNSIGNED_BYTE;
    }

    TextureParameters(TextureFormat format, TextureFilter filter, TextureWrap wrap, TextureDataType type)
        : format(format), filter(filter), wrap(wrap), type(type) {}

    // TextureParameters(TextureFilter filter) : format(TextureFormat::RGBA), filter(filter), wrap(TextureWrap::CLAMP) {}
    // TextureParameters(TextureFilter filter, TextureWrap wrap) : format(TextureFormat::RGBA), filter(filter), wrap(wrap) {}
};

class Texture {
  public:
    Texture(const std::string& name, const unsigned& width, const unsigned& height, const TextureParameters& tp);
    Texture(const std::string& name, SDL_Surface* surface, const TextureParameters& tp);
    virtual ~Texture() { glDeleteTextures(1, (GLuint*)&idTexture); }

    inline unsigned getWidth() const { return width; }
    inline unsigned getHeight() const { return height; }
    void bind(uint8_t slot) const;
    static void unbind(uint8_t slot);
    inline const std::string getName() const { return name; }
    inline const uint32_t getTextureID() const { return idTexture; }

  private:
    void init();
    unsigned width, height;
    GLuint idTexture;
    std::string name;
    TextureParameters textureParameters;
};

} // namespace Chimera
#endif
