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

enum class TextureFormat { NONE = 0, RGB = GL_RGB, RGBA = GL_RGBA, LUMINANCE = GL_LUMINANCE, LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA };

struct TextureParameters {
    TextureFormat format;
    TextureFilter filter;
    TextureWrap wrap;

    TextureParameters() {
        format = TextureFormat::RGBA;
        filter = TextureFilter::NEAREST;
        wrap = TextureWrap::REPEAT;
    }

    TextureParameters(TextureFormat format, TextureFilter filter, TextureWrap wrap) : format(format), filter(filter), wrap(wrap) {}
    TextureParameters(TextureFilter filter) : format(TextureFormat::RGBA), filter(filter), wrap(TextureWrap::CLAMP) {}
    TextureParameters(TextureFilter filter, TextureWrap wrap) : format(TextureFormat::RGBA), filter(filter), wrap(wrap) {}
};

class Texture {
  protected:
    Texture(const std::string& name, const unsigned& width, const unsigned& height)
        : name(name), width(width), height(height), idTexture(0) {}

  public:
    virtual ~Texture() { glDeleteTextures(1, (GLuint*)&idTexture); } //???

    inline unsigned getWidth() const { return width; }
    inline unsigned getHeight() const { return height; }
    void bind(uint8_t slot) const;
    void unbind(uint8_t slot) const;
    inline const std::string getName() const { return name; }

  protected:
    unsigned width;
    unsigned height;
    GLuint idTexture;
    std::string name;

    TextureParameters textureParameters;
};

class TextureFBO : public Texture {
  public:
    TextureFBO(const std::string& name, const unsigned& width, const unsigned& height);
    virtual ~TextureFBO() {}
    inline GLuint getFrameBufferId() const { return depthMapFBO; }

  protected:
    GLuint depthMapFBO;
};

class TextureSurface : public Texture {
  public:
    TextureSurface(const std::string& name, SDL_Surface* surface, const TextureParameters& tp);
    virtual ~TextureSurface() {}
};
} // namespace Chimera
#endif
