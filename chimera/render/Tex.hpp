#ifndef __CHIMERA_TEX__HPP
#define __CHIMERA_TEX__HPP

#include "Shader.hpp"
#include "chimera/OpenGLDefs.hpp"

#include "chimera/core/Singleton.hpp"
#include <map>

namespace Chimera {

enum class TEX_KIND { DIFFUSE = 0, SHADOWMAP = 1, SPECULAR = 2, EMISSIVE = 3 };

class Tex {
  public:
    Tex(const unsigned& _width, const unsigned& _height)
        : width(_width), height(_height), serial(++serialMaster), idTexture(0) {}
    virtual ~Tex() { glDeleteTextures(1, (GLuint*)&idTexture); }

    virtual bool init() {
        if (idTexture == 0) {
            glGenTextures(1, &idTexture);
            glBindTexture(GL_TEXTURE_2D, idTexture);
            return true;
        }

        return false;
    }

    void apply(const unsigned& _activeTexture, const std::string& _shaderPropName, Shader* _pShader) {
        glActiveTexture(GL_TEXTURE0 + (int)_activeTexture);
        glBindTexture(GL_TEXTURE_2D, idTexture);
        if (_pShader != nullptr)
            _pShader->setGlUniform1i(_shaderPropName.c_str(), _activeTexture);
    }

    inline unsigned getWidth() const { return width; }
    inline unsigned getHeight() const { return height; }
    inline unsigned getSerial() const { return serial; }

  protected:
    unsigned width;
    unsigned height;
    GLuint idTexture;

  private:
    unsigned serial;
    static unsigned serialMaster;
};

class TexFBO : public Tex {
  public:
    TexFBO(const unsigned& _width, const unsigned& _height) : Tex(_width, _height), depthMapFBO(0) {}
    virtual ~TexFBO() override;
    virtual bool init() override;
    inline GLuint getFrameBufferId() const { return depthMapFBO; }

  protected:
    GLuint depthMapFBO;
};

class TexImg : public Tex {
  public:
    TexImg(const std::string& _pathFile) : Tex(0, 0), pathFile(_pathFile) {}
    virtual ~TexImg() override;
    virtual bool init() override;

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
