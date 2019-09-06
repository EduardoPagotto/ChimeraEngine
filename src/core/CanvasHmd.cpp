#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/Exception.hpp"

#include "chimera/core/ShadersLoader.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CanvasHmd::CanvasHmd(const std::string& _title, int _width, int _height) : CanvasGL(_title, _width, _height, false) {
    fbo = 0;
    fb_tex = 0;
    fb_depth = 0;
    this->createFBO();
    this->createSquare();

} // namespace Chimera

CanvasHmd::~CanvasHmd() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &fb_tex);
    glDeleteRenderbuffers(1, &fb_depth);

    glDeleteBuffers(1, &quad_vertexbuffer);

    delete pShader;
}

void CanvasHmd::before() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    CanvasGL::before();
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CanvasHmd::after() {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->displayTexture();

    CanvasGL::after();
    // SDL_GL_SwapWindow(window);
}

glm::mat4 CanvasHmd::getPerspectiveProjectionMatrix(const float& _fov, const float& _near, const float& _far,
                                                    int _eye) {
    // void VideoDevice::executeViewPerspective ( const float &_fov,const float
    // &_near,const float &_far, int _eye ) {

    glViewport(0, 0, width, height);
    return glm::perspective(_fov, (GLfloat)(float)width / (float)height, _near, _far);

    // glMatrixMode ( GL_PROJECTION );
    // glLoadIdentity();
    // gluPerspective ( _fov, ( GLfloat ) ( float ) winSizeW / ( float ) winSizeH, _near,
    // _far ); glMatrixMode ( GL_MODELVIEW ); glLoadIdentity();
}

glm::mat4 CanvasHmd::getOrthoProjectionMatrix(int eyeIndex) {

    return glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
}

unsigned int CanvasHmd::next_pow2(unsigned int x) {
    x -= 1;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

void CanvasHmd::displayTexture() {

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glViewport(0, 0, width, height);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    pShader->link();
    // glUseProgram(quad_programID);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb_tex);
    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);

    // glUniform1f(timeID, (float)(glfwGetTime() * 10.0f));
    glUniform1f(timeID, (float)1.0f * 10.0f);

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

    // Swap buffers
    // glfwSwapBuffers(window);
    // glfwPollEvents();
}

void CanvasHmd::createSquare() {

    // The fullscreen quad's FBO
    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    };

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    Chimera::ShadersLoader sl;
    pShader = sl.loadShader("RenderToTex", "./libs/chimera/shader/Passthrough.vertexshader",
                            "./libs/chimera/shader/WobblyTexture.fragmentshader");

    GLuint texID = pShader->getUniformLocation("renderedTexture");
    GLuint timeID = pShader->getUniformLocation("time");

    // Create and compile our GLSL program from the shaders
    // GLuint quad_programID = LoadShaders("Passthrough.vertexshader", "WobblyTexture.fragmentshader");
    // GLuint texID = glGetUniformLocation(quad_programID, "renderedTexture");
    // GLuint timeID = glGetUniformLocation(quad_programID, "time");
}

void CanvasHmd::createFBO() {

    // refs:
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    // https://github.com/andersonfreitas/opengl-tutorial-org/blob/master/tutorial14_render_to_texture/tutorial14.cpp

    if (!fbo) {
        // pass 1 cria framebuffer // FramebufferName = 0; => fbo
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // pass 2 cria textura // renderedTexture => fb_tex
        glGenTextures(1, &fb_tex);
        // "Bind" the newly created texture : all future texture functions will modify this texture

        fbTexSize.w = next_pow2(width);
        fbTexSize.h = next_pow2(height);

        // create and attach the texture that will be used as a color buffer
        glBindTexture(GL_TEXTURE_2D, fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbTexSize.w, fbTexSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        // Filtro linear
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // pass 3  // The depth buffer //GLuint depthrenderbuffer => fb_depth
        glGenRenderbuffers(1, &fb_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbTexSize.w, fbTexSize.h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);

        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb_tex, 0);

        // Pass 4
        // Set "renderedTexture" as our colour attachement #0
        // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

        // Always check that our framebuffer is ok
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw Exception(std::string("Falha em instanciar o Frame Buffer"));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        printf("created render target: %dx%d (texture size: %dx%d)\n", width, height, fbTexSize.w, fbTexSize.h);
    }
}

// void CanvasHmd::initGL() { CanvasGL::initGL(); }
// void CanvasHmd::afterStart() { CanvasGL::afterStart(); }
// void CanvasHmd::restoreMatrix() { CanvasGL::restoreMatrix(); }
// void CanvasHmd::reshape(int _width, int _height) { CanvasGL::reshape(_width, _height) }
// void CanvasHmd::toggleFullScreen(){CanvasGL::toggleFullScreen()}

// void VideoDevice::perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear,
// GLdouble zFar )//TODO subistituir o executeViewPerspective
//{
//    const GLdouble pi = 3.1415926535897932384626433832795;
//    GLdouble fW, fH;
//
//    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
//    fH = tan( fovY / 360 * pi ) * zNear;
//    fW = fH * aspect;
//
//    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
//}

// void VideoDevice::executeViewOrto ( int eye ) {
//
//     glMatrixMode ( GL_PROJECTION );
//     glPushMatrix();
//     glLoadIdentity();
//     glOrtho ( 0, winSizeW, 0, winSizeH, -1, 1 );
//     glMatrixMode ( GL_MODELVIEW );
//     glPushMatrix();
//     glLoadIdentity();
//
// }

} // namespace Chimera
