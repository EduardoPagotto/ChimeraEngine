#pragma once
#include "Texture.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"
#include "chimera_core/gl/TextureParams.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3_image/SDL_image.h>
#include <cstddef>
#include <format>
#include <memory>
#include <stdexcept>

namespace ce {

    struct TextureLoader {
        using result_type = std::shared_ptr<Texture>;

        result_type operator()(const std::string& filepath, TexParam& tp) const {
            return TextureLoader::LoadFromFile(filepath, tp);
        }

        result_type operator()(SDL_Surface* surface, TexParam& tp) const { return CreateFromSurface(surface, tp); }

        static result_type LoadFromFile(const std::string& filepath, TexParam& tp) {
            // Carrega a imagem do disco usando a nova API do SDL3
            SDL_Surface* surface = IMG_Load(filepath.c_str());
            if (surface == nullptr) {
                throw std::runtime_error("[TextureLoader] Falha ao carregar imagem via SDL3_image: " +
                                         std::string(SDL_GetError()));
            }

            auto tex = CreateFromSurface(surface, tp);

            SDL_DestroySurface(surface);

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s",
                         std::format("[TextureLoader] Load id: {} size({} x {}) file: {}", tex->id, tex->width,
                                     tex->height, filepath)
                             .c_str());

            return tex;
        }

        // 2. NOVA FUNÇÃO AUXILIAR: Transforma qualquer SDL_Surface em Texture
        static result_type CreateFromSurface(SDL_Surface* surface, TexParam& tp) {
            if (surface == nullptr) {
                return nullptr;
            }

            // Garante o formato RGBA32 exigido pelo OpenGL
            SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
            if (converted == nullptr) {
                throw std::runtime_error("[TextureLoader] Falha ao converter superfície no SDL3: " +
                                         std::string(SDL_GetError()));
            }

            // const SDL_PixelFormatDetails* formatDetail = SDL_GetPixelFormatDetails(surface->format);
            // tp.format = (formatDetail->Amask != 0) ? TexFormat::RGBA : TexFormat::RGB;
            // tp.internalFormat = tp.format;

            InvertImageTexture(converted->pitch, converted->h, converted->pixels);

            uint32_t textureId = TextureLoader::Init(tp, converted);

            int w = converted->w;
            int h = converted->h;

            SDL_DestroySurface(converted);

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s",
                         std::format("[TextureLoader] Surface id: {} size({} x {})", textureId, w, h).c_str());

            return std::make_shared<Texture>(textureId, w, h);
        }

        static result_type CreateEmpty(const uint32_t& width, const uint32_t& height, const TexParam& tp) {
            // Geração da textura no hardware via OpenGL 4
            uint32_t textureId;
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);

            // Upload dos pixels da memória RAM para a VRAM
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(tp.format), static_cast<GLsizei>(width),
                         static_cast<GLsizei>(height), 0, static_cast<GLuint>(tp.format), static_cast<GLuint>(tp.type),
                         nullptr);

            TextureLoader::SetFilter(tp);

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s",
                         std::format("[TextureLoader] Empty id: {} size({} x {})", textureId, width, height).c_str());

            return std::make_shared<Texture>(textureId, width, height);
        }

      private:
        static void InvertImageTexture(int pitch, int height, void* image_pixels) {

            int index;
            void* temp_row;
            int height_div_2;

            temp_row = malloc(pitch);
            if (nullptr == temp_row) {
                throw std::runtime_error("[TextureLoader] Not enough memory for image inversion");
            }

            // if height is odd, don't need to swap middle row
            height_div_2 = (int)(height * .5);
            for (index = 0; index < height_div_2; index++) {
                // uses string.h
                std::memcpy((Uint8*)temp_row, (Uint8*)(image_pixels) + (static_cast<ptrdiff_t>(pitch * index)), pitch);

                std::memcpy((Uint8*)(image_pixels) + (static_cast<ptrdiff_t>(pitch * index)),
                            (Uint8*)(image_pixels) + (static_cast<ptrdiff_t>(pitch * (height - index - 1))), pitch);

                std::memcpy((Uint8*)(image_pixels) + (static_cast<ptrdiff_t>(pitch * (height - index - 1))), temp_row,
                            pitch);
            }
            free(temp_row);
        }

        static void SetFilter(const TexParam& tp) {
            // Configuração de filtros básicos (OpenGL 4)
            if (tp.minFilter != TexFilter::NONE) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(tp.minFilter));
            }

            if (tp.magFilter != TexFilter::NONE) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(tp.magFilter));
            }

            if (tp.wrap_r != TexWrap::NONE) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, static_cast<GLint>(tp.wrap_r));
            }

            if (tp.wrap_s != TexWrap::NONE) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(tp.wrap_s));
            }

            if (tp.wrap_t != TexWrap::NONE) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(tp.wrap_t));
            }
        }

        static uint32_t Init(TexParam& tp, SDL_Surface* converted) {
            // Geração da textura no hardware via OpenGL 4
            uint32_t textureId;
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);

            // Upload dos pixels da memória RAM para a VRAM
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(tp.format), converted->w, converted->h, 0,
                         static_cast<GLuint>(tp.format), static_cast<GLuint>(tp.type), converted->pixels);

            TextureLoader::SetFilter(tp);

            return textureId;
        }
    };

} // namespace ce
