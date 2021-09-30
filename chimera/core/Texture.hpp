#ifndef __CHIMERA_TEX__HPP
#define __CHIMERA_TEX__HPP

#include "chimera/core/Shader.hpp"

namespace Chimera {

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
