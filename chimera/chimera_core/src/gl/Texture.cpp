// #include "chimera_core/gl/Texture.hpp"
// #include "chimera_core/gl/OpenGLDefs.hpp"

namespace ce {

    // Texture::Texture(const uint32_t& width, const uint32_t& height, const TexParam& tp)
    //     : width(width), height(height), idTexture(0), textureParameters(tp) {

    //     this->init();
    //     glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(textureParameters.internalFormat),
    //     static_cast<GLint>(width),
    //                  static_cast<GLint>(height), 0, (GLuint)textureParameters.format, (GLuint)textureParameters.type,
    //                  nullptr);
    // }

    // Texture::Texture(SDL_Surface* surface, const TexParam& tp)
    //     : width(surface->w), height(surface->h), idTexture(0), textureParameters(tp) {

    //     this->init();

    //     const SDL_PixelFormatDetails* formatDetail = SDL_GetPixelFormatDetails(surface->format);
    //     textureParameters.format = (formatDetail->Amask != 0) ? TexFormat::RGBA : TexFormat::RGB;

    //     textureParameters.internalFormat = textureParameters.format;

    //     glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(textureParameters.format), surface->w, surface->h, 0,
    //                  (GLuint)textureParameters.format, (GLuint)textureParameters.type, surface->pixels);
    // }

    // void Texture::init() {
    //     glGenTextures(1, &idTexture);
    //     glBindTexture(GL_TEXTURE_2D, idTexture);

    //     if (textureParameters.minFilter != TexFilter::NONE) {
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(textureParameters.minFilter));
    //     }

    //     if (textureParameters.magFilter != TexFilter::NONE) {
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(textureParameters.magFilter));
    //     }

    //     if (textureParameters.wrap_r != TexWrap::NONE) {
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, static_cast<GLint>(textureParameters.wrap_r));
    //     }

    //     if (textureParameters.wrap_s != TexWrap::NONE) {
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(textureParameters.wrap_s));
    //     }

    //     if (textureParameters.wrap_t != TexWrap::NONE) {
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(textureParameters.wrap_t));
    //     }
    // }

    // void Texture::bind(const uint8_t& slot) const {
    //     glActiveTexture(GL_TEXTURE0 + slot);
    //     glBindTexture(GL_TEXTURE_2D, idTexture);
    // }

    // void Texture::Unbind(const uint8_t& slot) {
    //     glActiveTexture(GL_TEXTURE0 + slot);
    //     glBindTexture(GL_TEXTURE_2D, 0);
    // }

} // namespace ce
