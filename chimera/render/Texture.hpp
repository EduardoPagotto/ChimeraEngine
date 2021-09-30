#ifndef __CHIMERA_TEX__HPP
#define __CHIMERA_TEX__HPP

#include "chimera/core/Shader.hpp"

namespace Chimera {

#define SHADE_TEXTURE_DIFFUSE "material.tDiffuse"
#define SHADE_TEXTURE_SPECULA "material.tSpecular"
#define SHADE_TEXTURE_EMISSIVE "material.tEmissive"
#define SHADE_TEXTURE_SHADOW "shadowMap" // TODO: melhorar este nome no shader

class Texture {
  public:
    Texture(const std::string& _shadeName, const unsigned& _width, const unsigned& _height);
    virtual ~Texture() { glDeleteTextures(1, (GLuint*)&idTexture); }

    virtual bool init() {
        if (idTexture == 0) {
            glGenTextures(1, &idTexture);
            glBindTexture(GL_TEXTURE_2D, idTexture);
            return true;
        }

        return false;
    }

    void apply(Shader* _pShader) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, idTexture);
        if (_pShader != nullptr)
            _pShader->setUniform1i(shadeName.c_str(), index);
    }

    inline unsigned getWidth() const { return width; }
    inline unsigned getHeight() const { return height; }
    inline unsigned getSerial() const { return serial; }
    inline GLuint getIndex() const { return index; }

    inline const GLuint getID() const { return idTexture; }

    // TODO: reimplementar
    inline const std::string getName() const { return name; }

  protected:
    unsigned width;
    unsigned height;
    GLuint idTexture;

  private:
    std::string name;
    std::string shadeName;
    GLuint index;
    unsigned serial;
    static unsigned serialMaster;
};

class TextureFBO : public Texture {
  public:
    TextureFBO(const std::string& _shadeName, const unsigned& _width, const unsigned& _height)
        : Texture(_shadeName, _width, _height), depthMapFBO(0) {}
    virtual ~TextureFBO() override;
    virtual bool init() override;
    inline GLuint getFrameBufferId() const { return depthMapFBO; }

  protected:
    GLuint depthMapFBO;
};

class TextureImg : public Texture {
  public:
    TextureImg(const std::string& _shadeName, const std::string& _pathFile) : Texture(_shadeName, 0, 0), pathFile(_pathFile) {}
    virtual ~TextureImg() override;
    virtual bool init() override;

  private:
    int invert_image(int pitch, int height, void* image_pixels);

  protected:
    std::string pathFile;
};
} // namespace Chimera
#endif
