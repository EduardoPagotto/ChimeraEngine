#include <SDL2/SDL_image.h>

#include "chimera/core/Exception.hpp"
#include "chimera/render/Tex.hpp"

namespace Chimera {

unsigned Tex::serialMaster = 0;

Tex::Tex(const TEX_KIND& _kind, const unsigned& _width, const unsigned& _height)
    : width(_width), height(_height), serial(++serialMaster), idTexture(0), kind(_kind) {

    switch (_kind) {
        case TEX_KIND::DIFFUSE:
            shadePropName = SHADE_TEXTURE_DIFFUSE;
            break;
        case TEX_KIND::EMISSIVE:
            shadePropName = SHADE_TEXTURE_EMISSIVE;
            break;
        case TEX_KIND::SPECULAR:
            shadePropName = SHADE_TEXTURE_SPECULA;
            break;
        case TEX_KIND::SHADOWMAP:
            shadePropName = SHADE_TEXTURE_SHADOW;
            break;
        default:
            break;
    }
}

TexFBO::~TexFBO() {}

bool TexFBO::init() {

    if (Tex::init() == true) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glGenFramebuffers(1, &depthMapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, idTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return true;
    }
    return false;
}

//--------
TexImg::~TexImg() {}

bool TexImg::init() {
    if (Tex::init() == true) {
        SDL_Surface* pImage = IMG_Load(pathFile.c_str());
        if (pImage == nullptr)
            throw Exception("Falha ao ler arquivo:" + pathFile);

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

        return true;
    }
    return false;
}

// -----
// TexCentral::TexCentral() noexcept {}

// TexCentral::~TexCentral() {

//     std::map<std::string, TexImg*>::iterator it = mapTex.begin();
//     while (it != mapTex.end()) {

//         TexImg* pTex = it->second;
//         mapTex.erase(it);

//         delete pTex;
//         pTex = nullptr;

//         it = mapTex.begin();
//     }
// }

// void TexCentral::initAllTex() {
//     for (std::map<std::string, TexImg*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {
//         TexImg* pTex = iTex->second;
//         pTex->init();
//     }
// }

// TexImg* TexCentral::add(const std::string& name, const std::string& _pathFile) {

//     TexImg* pTex;
//     std::map<std::string, TexImg*>::iterator it = mapTex.find(name);
//     if (it != mapTex.end()) {
//         pTex = it->second;
//         return pTex;
//     }

//     pTex = new TexImg(_pathFile);
//     mapTex[name] = pTex;

//     return pTex;
// }

} // namespace Chimera