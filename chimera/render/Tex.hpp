#ifndef __CHIMERA_TEX__HPP
#define __CHIMERA_TEX__HPP

#include "chimera/core/Shader.hpp"

namespace Chimera {

#define SHADE_TEXTURE_DIFFUSE "material.tDiffuse"
#define SHADE_TEXTURE_SPECULA "material.tSpecular"
#define SHADE_TEXTURE_EMISSIVE "material.tEmissive"
#define SHADE_TEXTURE_SHADOW "shadowMap" // TODO: melhorar este nome no shader

enum class TEXTURE_KIND { DIFFUSE = 0, SHADOWMAP = 1, SPECULAR = 2, EMISSIVE = 3 };

class Texture {
  public:
    Texture(const TEXTURE_KIND& _kind, const unsigned& _width, const unsigned& _height);
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
        glActiveTexture(GL_TEXTURE0 + (int)kind);
        glBindTexture(GL_TEXTURE_2D, idTexture);
        if (_pShader != nullptr)
            _pShader->setGlUniform1i(shadePropName.c_str(), (int)kind);
    }

    inline TEXTURE_KIND getKind() const { return kind; }

    inline unsigned getWidth() const { return width; }
    inline unsigned getHeight() const { return height; }
    inline unsigned getSerial() const { return serial; }

  protected:
    unsigned width;
    unsigned height;
    GLuint idTexture;

    TEXTURE_KIND kind;
    std::string shadePropName;

  private:
    unsigned serial;
    static unsigned serialMaster;
};

class TextureFBO : public Texture {
  public:
    TextureFBO(const TEXTURE_KIND& _kind, const unsigned& _width, const unsigned& _height)
        : Texture(_kind, _width, _height), depthMapFBO(0) {}
    virtual ~TextureFBO() override;
    virtual bool init() override;
    inline GLuint getFrameBufferId() const { return depthMapFBO; }

  protected:
    GLuint depthMapFBO;
};

class TextureImg : public Texture {
  public:
    TextureImg(const TEXTURE_KIND& _kind, const std::string& _pathFile) : Texture(_kind, 0, 0), pathFile(_pathFile) {}
    virtual ~TextureImg() override;
    virtual bool init() override;

  private:
    int invert_image(int pitch, int height, void* image_pixels);

  protected:
    std::string pathFile;
};

// class TexCentral {
//     friend class Singleton<TexCentral>;

//   public:
//     void initAllTex();
//     TextureImg* add(const std::string& name, const std::string& _pathFile);

//   protected:
//     TexCentral() noexcept;
//     ~TexCentral();

//   private:
//     std::map<std::string, TextureImg*> mapTex;
// };

} // namespace Chimera
#endif
