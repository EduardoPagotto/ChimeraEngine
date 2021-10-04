#include "chimera/core/Texture.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

void Texture::bind(uint8_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, idTexture);
}

void Texture::unbind(uint8_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureFBO::TextureFBO(const std::string& name, const unsigned& width, const unsigned& height)
    : Texture(name, width, height), depthMapFBO(0) {

    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);

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
}

//--------
TextureSurface::TextureSurface(const std::string& name, SDL_Surface* surface, const TextureParameters& tp) : Texture(name, 0, 0) {

    textureParameters = tp;

    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);

    height = surface->h;
    width = surface->w;
    textureParameters.format = (surface->format->Amask != 0) ? TextureFormat::RGBA : TextureFormat::RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, (GLuint)textureParameters.format, surface->w, surface->h, 0, (GLuint)textureParameters.format,
                 GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLuint)textureParameters.filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLuint)textureParameters.filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLuint)textureParameters.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLuint)textureParameters.wrap);
}
} // namespace Chimera