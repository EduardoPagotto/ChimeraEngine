#pragma once
#include "chimera/core/OpenGLDefs.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {
enum class TexWrap {
    NONE = 0,
    REPEAT = GL_REPEAT,
    CLAMP = GL_CLAMP,
    MIRRORED = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};

enum class TexFilter { NONE = 0, LINEAR = GL_LINEAR, NEAREST = GL_NEAREST };

enum class TexFormat {
    NONE = 0,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    LUMINANCE = GL_LUMINANCE,
    LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
    // novos
    RGBA8 = GL_RGBA8,
    DEPTH24STENCIL8 = GL_DEPTH24_STENCIL8,
    RED_INTEGER = GL_RED_INTEGER,
    R32I = GL_R32I
};

enum class TexDType {
    NONE = GL_NONE,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    FLOAT = GL_FLOAT,
    INT = GL_INT
};

struct TexParam {
    TexParam() = default;
    TexParam(const TexParam& o) = default;
    TexParam(TexFormat format, TexFormat internalFormat, TexFilter minFilter, TexFilter magFilter, TexWrap wrap_r, TexWrap wrap_s,
             TexWrap wrap_t, TexDType type)
        : format(format), internalFormat(internalFormat), minFilter(minFilter), magFilter(magFilter), wrap_r(wrap_r), wrap_s(wrap_s),
          wrap_t(wrap_t), type(type), samples(1) {}

    TexFormat format = TexFormat::RGBA;
    TexFormat internalFormat = TexFormat::RGBA;
    TexFilter minFilter = TexFilter::NEAREST;
    TexFilter magFilter = TexFilter::NEAREST;
    TexWrap wrap_r = TexWrap::REPEAT;
    TexWrap wrap_s = TexWrap::REPEAT;
    TexWrap wrap_t = TexWrap::REPEAT;
    TexDType type = TexDType::UNSIGNED_BYTE;
    int samples = 1;
};

namespace Aux {
static void textureParameterSetUndefined(TexParam& val) {
    val.format = TexFormat::NONE;
    val.internalFormat = TexFormat::NONE;
    val.minFilter = TexFilter::NONE;
    val.magFilter = TexFilter::NONE;
    val.wrap_r = TexWrap::NONE;
    val.wrap_s = TexWrap::NONE;
    val.wrap_t = TexWrap::NONE;
    val.type = TexDType::NONE;
}

static bool textureParameterIsUndefined(const TexParam& val) {
    return (val.format == TexFormat::NONE && val.internalFormat == TexFormat::NONE);
}
} // namespace Aux

class Texture {
  public:
    Texture(const std::string& name, const unsigned& width, const unsigned& height, const TexParam& tp);
    Texture(const std::string& name, SDL_Surface* surface, const TexParam& tp);
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
    TexParam textureParameters;
};
} // namespace Chimera
