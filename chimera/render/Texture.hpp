#pragma once
#include "chimera/render/Shader.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

enum class TexWrap {
    NONE = 0,
    REPEAT = GL_REPEAT,
    CLAMP = GL_CLAMP,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
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
    TexFormat format;
    TexFormat internalFormat;
    TexFilter filter;
    TexWrap wrap;
    TexDType type;
    int samples;

    TexParam() {
        format = TexFormat::RGBA;
        internalFormat = TexFormat::RGBA;
        filter = TexFilter::NEAREST;
        wrap = TexWrap::REPEAT;
        type = TexDType::UNSIGNED_BYTE;
        samples = 1;
    }

    TexParam(TexFormat format, TexFormat internalFormat, TexFilter filter, TexWrap wrap, TexDType type)
        : format(format), internalFormat(internalFormat), filter(filter), wrap(wrap), type(type), samples(1) {}

    // TexParam(TexFilter filter) : format(TexFormat::RGBA), filter(filter), wrap(TexWrap::CLAMP) {}
    // TexParam(TexFilter filter, TexWrap wrap) : format(TexFormat::RGBA), filter(filter), wrap(wrap) {}
};

namespace Aux {
static void textureParameterSetUndefined(TexParam& val) {
    val.format = TexFormat::NONE;
    val.internalFormat = TexFormat::NONE;
    val.filter = TexFilter::NONE;
    val.wrap = TexWrap::NONE;
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
