#include "chimera_core/gl/buffer/FrameBuffer.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"
#include "chimera_core/gl/TextureLoader.hpp"

namespace ce {

    namespace Aux {

        static bool is_depth_format(TexFormat format) {
            switch (format) {
                case TexFormat::DEPTH_COMPONENT:
                case TexFormat::DEPTH24STENCIL8:
                    return true;
                default:
                    break;
            }

            return false;
        }

    } // namespace Aux

    static const uint32_t max_frame_buffer_size = 8192;

    FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec) : fram_buffer_id_(0), rbo_(0), spec_(spec) {

        Aux::texture_parameter_set_undefined(rbo_spec_);
        Aux::texture_parameter_set_undefined(depth_tex_spec_);

        for (const TexParam& tex_parm : spec.attachments) {

            if (!Aux::is_depth_format(tex_parm.format)) {
                color_tex_specs_.emplace_back(tex_parm); // color only
            } else {
                // if has filter parameters them is a texture
                if ((tex_parm.minFilter != TexFilter::NONE) && (tex_parm.magFilter != TexFilter::NONE)) {
                    depth_tex_spec_ = tex_parm; // depth texture
                } else {                        //
                    rbo_spec_ = tex_parm;       // is a rbo
                }
            }
        }

        this->invalidade();
    }

    FrameBuffer::~FrameBuffer() { this->destroy(); }

    void FrameBuffer::destroy() {

        if (fram_buffer_id_ != 0U) {
            glDeleteFramebuffers(1, &fram_buffer_id_);
            fram_buffer_id_ = 0;

            if (!color_attachments_.empty()) {
                for (size_t i = 0; i < color_attachments_.size(); i++) {
                    auto tex = color_attachments_[i];
                    tex.reset();
                }
            }

            color_attachments_.clear();

            if (rbo_ != 0) {
                glDeleteRenderbuffers(1, &rbo_);
                rbo_ = 0;
            }

            rbo_ = 0;
        }
    }

    void FrameBuffer::invalidade() {
        this->destroy();

        glGenFramebuffers(1, &fram_buffer_id_);
        glBindFramebuffer(GL_FRAMEBUFFER, fram_buffer_id_);

        // Attachment color
        if (color_tex_specs_.size() > 0) {
            color_attachments_.reserve(color_tex_specs_.size());

            int index = 0;
            for (const TexParam& texture_param : color_tex_specs_) {
                // const TexParam& textureParam = cas.textureParameters;

                std::shared_ptr<Texture> tex = TextureLoader::create_empty(spec_.width, spec_.height, texture_param);
                color_attachments_.emplace_back(tex);

                const uint32_t t_id = tex->id;

                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, t_id, 0);

                index++;
            }
        }

        if (color_attachments_.size() > 1) {
            // TODO: verificar se < de 4
            GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
                                 GL_COLOR_ATTACHMENT3};
            glDrawBuffers(color_attachments_.size(), buffers);
        } else if (color_attachments_.empty()) {
            // so depth-pass
            glDrawBuffer(GL_NONE);
        }

        // depth Texture
        if (!Aux::texture_parameter_is_undefined(depth_tex_spec_)) {

            depth_attachment_ = TextureLoader::create_empty(spec_.width, spec_.height, depth_tex_spec_);

            GLfloat border_color[] = {1.0, 1.0, 1.0, 1.0};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_attachment_->id, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        // depth R.B.O.
        if (!Aux::texture_parameter_is_undefined(rbo_spec_)) {

            TexFormat tf = rbo_spec_.format;          // GL_DEPTH_COMPONENT
            TexFormat tfi = rbo_spec_.internalFormat; // GL_DEPTH_ATTACHMENT

            glGenRenderbuffers(1, &rbo_);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
            glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)tf, spec_.width, spec_.height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)tfi, GL_RENDERBUFFER, rbo_);
        }

        // Always check that our framebuffer is ok
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::string("Falha em instanciar o Frame Buffer");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::bind() const {
        glViewport(0, 0, spec_.width, spec_.height);
        glBindFramebuffer(GL_FRAMEBUFFER, fram_buffer_id_);

        GLbitfield mask = 0;
        if (!Aux::texture_parameter_is_undefined(rbo_spec_) || !Aux::texture_parameter_is_undefined(depth_tex_spec_)) {
            mask |= GL_DEPTH_BUFFER_BIT;
        }

        if (color_attachments_.size() > 1) {
            mask |= GL_COLOR_BUFFER_BIT;
        }

        glClear(mask);
    }

    void FrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void FrameBuffer::resize(const uint32_t& width, const uint32_t& height) {

        if (width == 0 || height == 0 || width > max_frame_buffer_size || height > max_frame_buffer_size) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Framebuffer resize erros Erro");
            return;
        }

        spec_.width = width;
        spec_.height = height;
        this->invalidade();
    }

    int FrameBuffer::read_pixel(uint32_t attachment_index, int x, int y) {

        int pixel_data = 0;
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data); // FIXME: ver com o TexDType!!!!!!

        return pixel_data;
    }

    void FrameBuffer::clear_attachment(uint32_t attachment_index, const int value) {

        const TexParam& tp = color_tex_specs_[attachment_index];
        const TexFormat& tf = tp.format;

        glClearTexImage(color_attachments_[attachment_index]->id, 0, (GLenum)tf, GL_INT, &value);
    }
} // namespace ce
