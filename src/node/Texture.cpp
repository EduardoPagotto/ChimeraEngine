#ifdef WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include "chimera/core/ExceptionChimera.hpp"

#include "chimera/node/Texture.hpp"

namespace Chimera {

Texture::Texture(const std::string& _name, const TEX_SEQ& _indexTextureSeq, const std::string& _pathFile)
    : Entity(EntityKind::TEXTURE, _name) {

    indexTextureSeq = _indexTextureSeq;
    texturaCarregada = false;
    pathFile = _pathFile;
    idTexture = 0;

    height = 0;
    width = 0;

    depthMapFBO = 0;

    refCount = 0;

    log = spdlog::get("chimera");
    log->debug("Constructor textura nome:{} arquivo:{}", _name, _pathFile);
}

Texture::Texture(const std::string& _name, const TEX_SEQ& _indexTextureSeq, const unsigned& _width,
                 const unsigned& _height)
    : Entity(EntityKind::TEXTURE, _name) {

    indexTextureSeq = _indexTextureSeq;
    texturaCarregada = false;
    pathFile = "FRAME_BUFFER";
    depthMapFBO = 0;
    height = _height;
    width = _width;

    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    refCount = 0;

    log = spdlog::get("chimera");
    log->debug("Constructor textura nome:{0} vazia: {1:03d}x{2:03d}", _name, _width, _height);
}

Texture::~Texture() { glDeleteTextures(1, (GLuint*)&idTexture); }

void Texture::apply() {

    glActiveTexture(GL_TEXTURE0 + (unsigned)indexTextureSeq);
    glBindTexture(GL_TEXTURE_2D, idTexture);
}

void Texture::init() {

    if (texturaCarregada == false) {

        if (pathFile.compare("FRAME_BUFFER") != 0) {

            SDL_Surface* pImage = IMG_Load(pathFile.c_str());
            if (pImage == nullptr)
                throw ExceptionChimera("Falha ao ler arquivo:" + pathFile);

            // Create The Texture
            glGenTextures(1, (GLuint*)&idTexture);

            // Load in texture
            glBindTexture(GL_TEXTURE_2D,
                          idTexture); // Typical Texture Generation Using Data From The Bitmap

            height = pImage->h;
            width = pImage->w;

            // Generate The Texture
            if (pImage->format->Amask != 0) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,
                             pImage->pixels);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE,
                             pImage->pixels);
            }

            // Nearest Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            SDL_FreeSurface(pImage);

        } else {

            glBindTexture(GL_TEXTURE_2D, idTexture);

            glGenFramebuffers(1, &depthMapFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, idTexture, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        texturaCarregada = true;
        log->info("Instanciada textura Nome:{0} id:{1:d} num instancias:{2:d}", getName(), idTexture, getRefCount());
    }
}
} // namespace Chimera
