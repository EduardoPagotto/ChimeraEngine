#include "chimera/core/visible/Texture.hpp"

namespace Chimera {

Texture::Texture(const std::string& name, const unsigned& width, const unsigned& height, const TexParam& tp)
    : name(name), width(width), height(height), idTexture(0), textureParameters(tp) {

    this->init();
    glTexImage2D(GL_TEXTURE_2D, 0, (GLuint)textureParameters.internalFormat, width, height, 0, (GLuint)textureParameters.format,
                 (GLuint)textureParameters.type, nullptr);
}

Texture::Texture(const std::string& name, SDL_Surface* surface, const TexParam& tp)
    : name(name), width(surface->w), height(surface->h), idTexture(0), textureParameters(tp) {

    this->init();
    textureParameters.format = (surface->format->Amask != 0) ? TexFormat::RGBA : TexFormat::RGB;
    textureParameters.internalFormat = textureParameters.format;
    glTexImage2D(GL_TEXTURE_2D, 0, (GLuint)textureParameters.format, surface->w, surface->h, 0, (GLuint)textureParameters.format,
                 (GLuint)textureParameters.type, surface->pixels);
}

void Texture::init() {
    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLuint)textureParameters.filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLuint)textureParameters.filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, (GLuint)textureParameters.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLuint)textureParameters.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLuint)textureParameters.wrap);
}

void Texture::bind(uint8_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, idTexture);
}

void Texture::unbind(uint8_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace Chimera