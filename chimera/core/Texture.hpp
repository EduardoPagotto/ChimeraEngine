#ifndef __CHIMERA_TEX__HPP
#define __CHIMERA_TEX__HPP

#include "chimera/core/Shader.hpp"

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

    virtual bool init() {
        if (idTexture == 0) {
            glGenTextures(1, &idTexture);
            glBindTexture(GL_TEXTURE_2D, idTexture);
            return true;
        }

        return false;
    }

    inline unsigned getWidth() const { return width; }
    inline unsigned getHeight() const { return height; }
    inline const GLuint getID() const { return idTexture; }
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
    TextureFBO(const std::string& name, const unsigned& width, const unsigned& height) : Texture(name, width, height), depthMapFBO(0) {}
    virtual ~TextureFBO() {}
    virtual bool init() override;
    inline GLuint getFrameBufferId() const { return depthMapFBO; }

  protected:
    GLuint depthMapFBO;
};

class TextureImg : public Texture {
  public:
    TextureImg(const std::string& name, const std::string& _pathFile) : Texture(name, 0, 0), pathFile(_pathFile) {}
    virtual ~TextureImg() {}
    virtual bool init() override;

  private:
    int invert_image(int pitch, int height, void* image_pixels);

  protected:
    std::string pathFile;
};
} // namespace Chimera
#endif
