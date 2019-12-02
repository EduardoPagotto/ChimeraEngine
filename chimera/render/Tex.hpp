#ifndef __CHIMERA_TEX__HPP
#define __CHIMERA_TEX__HPP

#include "chimera/core/Shader.hpp"

namespace Chimera {

#define SHADE_TEXTURE_DIFFUSE "material.tDiffuse"
#define SHADE_TEXTURE_SPECULA "material.tSpecular"
#define SHADE_TEXTURE_EMISSIVE "material.tEmissive"
#define SHADE_TEXTURE_SHADOW "shadowMap" // TODO: melhorar este nome no shader

enum class TEX_KIND { DIFFUSE = 0, SHADOWMAP = 1, SPECULAR = 2, EMISSIVE = 3 };

class Tex {
  public:
    Tex(const TEX_KIND& _kind, const unsigned& _width, const unsigned& _height);
    virtual ~Tex() { glDeleteTextures(1, (GLuint*)&idTexture); }

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

    inline TEX_KIND getKind() const { return kind; }

    inline unsigned getWidth() const { return width; }
    inline unsigned getHeight() const { return height; }
    inline unsigned getSerial() const { return serial; }

  protected:
    unsigned width;
    unsigned height;
    GLuint idTexture;

    TEX_KIND kind;
    std::string shadePropName;

  private:
    unsigned serial;
    static unsigned serialMaster;
};

class TexFBO : public Tex {
  public:
    TexFBO(const TEX_KIND& _kind, const unsigned& _width, const unsigned& _height)
        : Tex(_kind, _width, _height), depthMapFBO(0) {}
    virtual ~TexFBO() override;
    virtual bool init() override;
    inline GLuint getFrameBufferId() const { return depthMapFBO; }

  protected:
    GLuint depthMapFBO;
};

class TexImg : public Tex {
  public:
    TexImg(const TEX_KIND& _kind, const std::string& _pathFile) : Tex(_kind, 0, 0), pathFile(_pathFile) {}
    virtual ~TexImg() override;
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
//     TexImg* add(const std::string& name, const std::string& _pathFile);

//   protected:
//     TexCentral() noexcept;
//     ~TexCentral();

//   private:
//     std::map<std::string, TexImg*> mapTex;
// };

} // namespace Chimera
#endif
