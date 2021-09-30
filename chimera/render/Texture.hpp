#ifndef __CHIMERA_TEX__HPP
#define __CHIMERA_TEX__HPP

#include "chimera/core/Shader.hpp"

namespace Chimera {

class Texture {
  public:
    Texture(const unsigned& _width, const unsigned& _height);
    virtual ~Texture() { glDeleteTextures(1, (GLuint*)&idTexture); }

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
    inline unsigned getSerial() const { return serial; }
    inline const GLuint getID() const { return idTexture; }
    inline const std::string getName() const { return name; }

  protected:
    unsigned width;
    unsigned height;
    GLuint idTexture;

  private:
    std::string name;
    unsigned serial;
    static unsigned serialMaster;
};

class TextureFBO : public Texture {
  public:
    TextureFBO(const unsigned& _width, const unsigned& _height) : Texture(_width, _height), depthMapFBO(0) {}
    virtual ~TextureFBO() override;
    virtual bool init() override;
    inline GLuint getFrameBufferId() const { return depthMapFBO; }

  protected:
    GLuint depthMapFBO;
};

class TextureImg : public Texture {
  public:
    TextureImg(const std::string& _pathFile) : Texture(0, 0), pathFile(_pathFile) {}
    virtual ~TextureImg() override;
    virtual bool init() override;

  private:
    int invert_image(int pitch, int height, void* image_pixels);

  protected:
    std::string pathFile;
};
} // namespace Chimera
#endif
