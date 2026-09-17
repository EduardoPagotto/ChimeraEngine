#pragma once
#include "chimera_core/gl/OpenGLDefs.hpp"
#include <SDL3/SDL.h>

namespace ce {

    struct Texture {
        GLuint id;
        uint32_t width;
        uint32_t height;

        explicit Texture(const GLuint& id, const uint32_t& width, const uint32_t& height)
            : id(id), width(width), height(height) {}

        Texture(Texture&& other) noexcept : id(other.id), width(other.width), height(other.height) { other.id = 0; }

        virtual ~Texture() { glDeleteTextures(1, (&id)); }

        // Texture() = delete;
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        void bind(const uint8_t& slot) const {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, id);
        }

        static void Unbind(const uint8_t& slot) {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    };
} // namespace ce
