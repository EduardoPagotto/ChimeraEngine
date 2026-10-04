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
#include <vector>

namespace ce {

    struct TextureLoader {
        using result_type = std::shared_ptr<Texture>;

        result_type operator()(const std::string& filepath, TexParam& tp) const {
            return TextureLoader::load_from_file(filepath, tp);
        }

        result_type operator()(SDL_Surface* surface, TexParam& tp) const { return create_from_surface(surface, tp); }

        static result_type load_from_file(const std::string& filepath, TexParam& tp) {
            // Carrega a imagem do disco usando a nova API do SDL3
            SDL_Surface* surface = IMG_Load(filepath.c_str());
            if (surface == nullptr) {
                throw std::runtime_error("[TextureLoader] Falha ao carregar imagem via SDL3_image: " +
                                         std::string(SDL_GetError()));
            }

            auto tex = create_from_surface(surface, tp);

            SDL_DestroySurface(surface);

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s",
                         std::format("[TextureLoader] Load id: {} size({} x {}) file: {}", tex->id, tex->width,
                                     tex->height, filepath)
                             .c_str());

            return tex;
        }

        // 2. NOVA FUNÇÃO AUXILIAR: Transforma qualquer SDL_Surface em Texture
        static result_type create_from_surface(SDL_Surface* surface, TexParam& tp) {
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

            invert_image_texture(converted->pitch, converted->h, converted->pixels);

            uint32_t texture_id = TextureLoader::init(tp, converted);

            int w = converted->w;
            int h = converted->h;

            SDL_DestroySurface(converted);

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s",
                         std::format("[TextureLoader] Surface id: {} size({} x {})", texture_id, w, h).c_str());

            return std::make_shared<Texture>(texture_id, w, h);
        }

        static result_type create_empty(const uint32_t& width, const uint32_t& height, const TexParam& tp) {
            // Geração da textura no hardware via OpenGL 4
            uint32_t texture_id;
            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);

            // Upload dos pixels da memória RAM para a VRAM
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(tp.format), static_cast<GLsizei>(width),
                         static_cast<GLsizei>(height), 0, static_cast<GLuint>(tp.format), static_cast<GLuint>(tp.type),
                         nullptr);

            TextureLoader::set_filter(tp);

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s",
                         std::format("[TextureLoader] Empty id: {} size({} x {})", texture_id, width, height).c_str());

            return std::make_shared<Texture>(texture_id, width, height);
        }

        static void create_file_from_surface(SDL_Surface* surface, const std::string& pathfile) {

            if (IMG_SavePNG(surface, pathfile.c_str())) {
                SDL_Log("[TextureLoader] Saved: %s (%d x %d)", pathfile.c_str(), surface->w, surface->h);
            } else {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[TextureLoader] Fail to save: %s, erro: %s ",
                             pathfile.c_str(), SDL_GetError());
            }
        }

        static bool create_file_from_texture_gl(GLuint texture_id, uint32_t width, uint32_t height,
                                                const std::string& pathfile) {
            // 1. Alocar buffer para receber os pixels da GPU (RGBA8888 -> 4 bytes por pixel)
            std::vector<uint8_t> pixels(static_cast<size_t>(width * height * 4));

            // 2. Criar e vincular um Framebuffer temporário para ler a textura
            GLuint fbo = 0;
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

            // Verificar se o FBO foi criado corretamente
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[TextureLoader] Falha ao vincular textura ao Framebuffer.");

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glDeleteFramebuffers(1, &fbo);
                return false;
            }

            // 3. Ler os pixels da textura para o vetor na RAM
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

            // Limpar o FBO da memória
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteFramebuffers(1, &fbo);

            // 4. Inverter o buffer verticalmente (OpenGL é Bottom-Up, SDL é Top-Down)
            std::vector<uint8_t> pixels_invertidos(width * height * 4);
            int row_size = width * 4;
            for (int y = 0; y < height; ++y) {
                // Copia a linha de baixo do original para a linha de cima do invertido
                std::copy(pixels.begin() + (y * row_size), pixels.begin() + ((y + 1) * row_size),
                          pixels_invertidos.begin() + ((height - 1 - y) * row_size));
            }

            // 5. Criar uma SDL_Surface a partir dos pixels invertidos usando a API do SDL3
            // Nota: O pitch é a largura em bytes (width * 4)
            SDL_Surface* surface =
                SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA8888, pixels_invertidos.data(), row_size);

            if (surface == nullptr) {

                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[TextureLoader] Erro ao criar SDL_Surface: %s",
                             SDL_GetError());

                return false;
            }

            TextureLoader::create_file_from_surface(surface, pathfile);

            SDL_DestroySurface(surface);

            return true;
        }

      private:
        static void invert_image_texture(int pitch, int height, void* image_pixels) {

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

        static void set_filter(const TexParam& tp) {
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

        static uint32_t init(TexParam& tp, SDL_Surface* converted) {
            // Geração da textura no hardware via OpenGL 4
            uint32_t texture_id;
            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);

            // Upload dos pixels da memória RAM para a VRAM
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(tp.format), converted->w, converted->h, 0,
                         static_cast<GLuint>(tp.format), static_cast<GLuint>(tp.type), converted->pixels);

            TextureLoader::set_filter(tp);

            return texture_id;
        }
    };

} // namespace ce
