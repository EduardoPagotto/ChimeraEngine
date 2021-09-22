#include "chimera/render/Texture.hpp"
#include "chimera/core/Exception.hpp"
#include <SDL2/SDL_image.h>

namespace Chimera {

unsigned Texture::serialMaster = 0;

Texture::Texture(const std::string& _shadeName, const unsigned& _width, const unsigned& _height)
    : width(_width), height(_height), serial(++serialMaster), idTexture(0), shadeName(_shadeName) {

    index = 0;
    if (_shadeName.compare(SHADE_TEXTURE_DIFFUSE) == 0)
        index = 0;
    else if (_shadeName.compare(SHADE_TEXTURE_SHADOW) == 0)
        index = 1;
    else if (_shadeName.compare(SHADE_TEXTURE_SPECULA) == 0)
        index = 2;
    else if (_shadeName.compare(SHADE_TEXTURE_EMISSIVE) == 0)
        index = 3;
}

TextureFBO::~TextureFBO() {}

bool TextureFBO::init() {

    if (Texture::init() == true) {
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
TextureImg::~TextureImg() {}

int TextureImg::invert_image(int pitch, int height, void* image_pixels) {
    int index;
    void* temp_row;
    int height_div_2;

    temp_row = (void*)malloc(pitch);
    if (NULL == temp_row) {
        SDL_SetError("Not enough memory for image inversion");
        return -1;
    }
    // if height is odd, don't need to swap middle row
    height_div_2 = (int)(height * .5);
    for (index = 0; index < height_div_2; index++) {
        // uses string.h
        memcpy((Uint8*)temp_row, (Uint8*)(image_pixels) + pitch * index, pitch);

        memcpy((Uint8*)(image_pixels) + pitch * index, (Uint8*)(image_pixels) + pitch * (height - index - 1), pitch);
        memcpy((Uint8*)(image_pixels) + pitch * (height - index - 1), temp_row, pitch);
    }
    free(temp_row);
    return 0;
}

bool TextureImg::init() {
    if (Texture::init() == true) {
        SDL_Surface* pImage = IMG_Load(pathFile.c_str());
        if (pImage == nullptr)
            throw Exception("Falha ao ler arquivo:" + pathFile);

        if (invert_image(pImage->pitch, pImage->h, pImage->pixels) != 0) {
            SDL_SetError("Falha na inversao de pixels");
        }

        height = pImage->h;
        width = pImage->w;

        // Generate The Texture
        if (pImage->format->Amask != 0) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pImage->pixels);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->w, pImage->h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pImage->pixels);
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

//     std::map<std::string, TextureImg*>::iterator it = mapTex.begin();
//     while (it != mapTex.end()) {

//         TextureImg* pTex = it->second;
//         mapTex.erase(it);

//         delete pTex;
//         pTex = nullptr;

//         it = mapTex.begin();
//     }
// }

// void TexCentral::initAllTex() {
//     for (std::map<std::string, TextureImg*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {
//         TextureImg* pTex = iTex->second;
//         pTex->init();
//     }
// }

// TextureImg* TexCentral::add(const std::string& name, const std::string& _pathFile) {

//     TextureImg* pTex;
//     std::map<std::string, TextureImg*>::iterator it = mapTex.find(name);
//     if (it != mapTex.end()) {
//         pTex = it->second;
//         return pTex;
//     }

//     pTex = new TextureImg(_pathFile);
//     mapTex[name] = pTex;

//     return pTex;
// }

} // namespace Chimera