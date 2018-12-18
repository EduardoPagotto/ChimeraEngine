#ifndef __CHIMERA_TEXTURE__HPP
#define __CHIMERA_TEXTURE__HPP

#include "Entity.hpp"
#include "chimera/core/OpenGLDefs.hpp"

#include <spdlog/spdlog.h>

namespace Chimera {

enum class TEX_SEQ { DIFFUSE = 0, SHADOWMAP = 1, SPECULAR = 2, EMISSIVE = 3 };

class Texture : public Entity {
  public:
    Texture(const std::string& _name, const TEX_SEQ& _indexTextureSeq, const std::string& _pathFile);

    virtual ~Texture();

    void apply();

    void init();

    unsigned getWidth() const { return width; }

    unsigned getHeight() const { return height; }

    unsigned int getRefCount() const { return refCount; }

    void addRefCount() { refCount++; }

    void releaseRefCount() {
        if (refCount > 0)
            refCount--;
    }

    GLuint getFrameBufferId() const { return depthMapFBO; }

    TEX_SEQ getIndexTextureSeq() const { return indexTextureSeq; }

  private:
    TEX_SEQ indexTextureSeq;

    GLuint depthMapFBO;

    unsigned int refCount;

    std::string pathFile;

    unsigned width;
    unsigned height;

    GLuint idTexture;
    bool texturaCarregada;

    std::shared_ptr<spdlog::logger> log;
};
} // namespace Chimera
#endif
