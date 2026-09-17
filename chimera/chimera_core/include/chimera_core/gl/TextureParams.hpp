#pragma once

#include "chimera_core/gl/OpenGLDefs.hpp"

namespace ce {
    enum class TexWrap {
        NONE = 0,
        REPEAT = GL_REPEAT,
        CLAMP = GL_CLAMP_TO_EDGE,
        MIRRORED = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };

    enum class TexFilter { NONE = 0, LINEAR = GL_LINEAR, NEAREST = GL_NEAREST };

    enum class TexFormat {
        NONE = 0,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        LUMINANCE = GL_LUMINANCE,
        LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
        DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
        DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
        // novos
        RGBA8 = GL_RGBA8,
        DEPTH24STENCIL8 = GL_DEPTH24_STENCIL8,
        RED_INTEGER = GL_RED_INTEGER,
        R32I = GL_R32I
    };

    enum class TexDType {
        NONE = GL_NONE,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        FLOAT = GL_FLOAT,
        INT = GL_INT
    };

    struct TexParam {
        TexParam() = default;
        TexParam(const TexParam& o) = default;
        TexParam(TexFormat format, TexFormat internalFormat, TexFilter minFilter, TexFilter magFilter, TexWrap wrap_r,
                 TexWrap wrap_s, TexWrap wrap_t, TexDType type)
            : format(format), internalFormat(internalFormat), minFilter(minFilter), magFilter(magFilter),
              wrap_r(wrap_r), wrap_s(wrap_s), wrap_t(wrap_t), type(type), samples(1) {}

        TexFormat format = TexFormat::RGBA;
        TexFormat internalFormat = TexFormat::RGBA;
        TexFilter minFilter = TexFilter::NEAREST;
        TexFilter magFilter = TexFilter::NEAREST;
        TexWrap wrap_r = TexWrap::REPEAT;
        TexWrap wrap_s = TexWrap::REPEAT;
        TexWrap wrap_t = TexWrap::REPEAT;
        TexDType type = TexDType::UNSIGNED_BYTE;
        int samples = 1;
    };

    namespace Aux {
        [[maybe_unused]]
        static void textureParameterSetUndefined(TexParam& val) {
            val.format = TexFormat::NONE;
            val.internalFormat = TexFormat::NONE;
            val.minFilter = TexFilter::NONE;
            val.magFilter = TexFilter::NONE;
            val.wrap_r = TexWrap::NONE;
            val.wrap_s = TexWrap::NONE;
            val.wrap_t = TexWrap::NONE;
            val.type = TexDType::NONE;
        }

        [[maybe_unused]]
        static bool textureParameterIsUndefined(const TexParam& val) {
            return (val.format == TexFormat::NONE && val.internalFormat == TexFormat::NONE);
        }
    } // namespace Aux

    inline void Invert_image_texture(int pitch, int height, void* image_pixels) {

        int index;
        void* temp_row;
        int height_div_2;

        temp_row = malloc(pitch);
        if (nullptr == temp_row) {
            throw std::string("Not enough memory for image inversion");
        }

        // if height is odd, don't need to swap middle row
        height_div_2 = (int)(height * .5);
        for (index = 0; index < height_div_2; index++) {
            // uses string.h
            std::memcpy((Uint8*)temp_row, (Uint8*)(image_pixels) + pitch * index, pitch);

            std::memcpy((Uint8*)(image_pixels) + pitch * index, (Uint8*)(image_pixels) + pitch * (height - index - 1),
                        pitch);
            std::memcpy((Uint8*)(image_pixels) + pitch * (height - index - 1), temp_row, pitch);
        }
        free(temp_row);
    }

} // namespace ce
