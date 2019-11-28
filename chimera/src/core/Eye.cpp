#include "chimera/core/Eye.hpp"
#include "chimera/core/Exception.hpp"
//#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>

namespace Chimera {

Eye::Eye(const unsigned short& _indexEye, const int& _w, const int& _h, Shader* _pShader) {

    this->fbo = 0;
    this->fb_tex = 0;
    this->fb_depth = 0;
    this->quad_vertexbuffer = 0;

    this->indexEye = _indexEye;
    this->pShader = _pShader;

    this->fbTexSize.w = next_pow2(_w);
    this->fbTexSize.h = next_pow2(_h);
    this->createFBO();
    this->createSquare();

    // this->posInitW = _indexEye == 0 ? 0 : _w;
    if (_indexEye == 0) {
        this->posInitW = 0;
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Left Eye Setup pos:%d size:%d", this->posInitW, this->fbTexSize.w);
    } else {
        this->posInitW = this->fbTexSize.w;
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Right Eye Setup pos:%d size:%d", this->posInitW, this->fbTexSize.w);
    }
}

Eye::~Eye() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &fb_tex);
    glDeleteRenderbuffers(1, &fb_depth);

    glDeleteBuffers(1, &quad_vertexbuffer);
}

unsigned int Eye::next_pow2(unsigned int x) {
    x -= 1;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

void Eye::begin() { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }

void Eye::end() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

glm::mat4 Eye::getPerspectiveProjectionMatrix(ViewPoint* vp) {
    glViewport(0, 0, fbTexSize.w, fbTexSize.h);
    return glm::perspective(vp->fov, (GLfloat)(float)fbTexSize.w / (float)fbTexSize.h, vp->near, vp->far);
}

glm::mat4 Eye::getOrthoProjectionMatrix() {
    return glm::ortho(0.0f, static_cast<GLfloat>(fbTexSize.w), 0.0f, static_cast<GLfloat>(fbTexSize.h));
}

void Eye::createFBO() {

    // refs:
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    // https://github.com/andersonfreitas/opengl-tutorial-org/blob/master/tutorial14_render_to_texture/tutorial14.cpp

    if (!fbo) {
        // pass 1 cria framebuffer // FramebufferName => fbo
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // pass 2 cria textura that will be used as a color buffer // renderedTexture => fb_tex
        // create and attach the texture
        glGenTextures(1, &fb_tex);
        glBindTexture(GL_TEXTURE_2D, fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbTexSize.w, fbTexSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        // Filtro linear
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // pass 3  // The depth buffer // depthrenderbuffer => fb_depth
        glGenRenderbuffers(1, &fb_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbTexSize.w, fbTexSize.h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);

        // Pass 4
        // Set "renderedTexture" as our colour attachement #0
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb_tex, 0);

        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

        // Always check that our framebuffer is ok
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw Exception(std::string("Falha em instanciar o Frame Buffer"));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "created render target: %dx%d", fbTexSize.w, fbTexSize.h);
    }
}

void Eye::createSquare() {

    // The fullscreen quad's FBO
    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    };

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint texID = pShader->getUniformLocation("renderedTexture");
    // GLuint timeID = pShader->getUniformLocation("time");
}

void Eye::displayTexture() {

    // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glViewport(this->posInitW, 0, fbTexSize.w, fbTexSize.h);

    // Clear the screen
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    pShader->link();
    // glUseProgram(quad_programID);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb_tex);
    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);

    // glUniform1f(timeID, (float)(glfwGetTime() * 10.0f));
    // glUniform1f(timeID, (float)1.0f * 10.0f);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

    glDisableVertexAttribArray(0);

    pShader->unlink();
}

} // namespace Chimera