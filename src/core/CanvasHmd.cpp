#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/Exception.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CanvasHmd::CanvasHmd(const std::string& _title, int _width, int _height) : CanvasGL(_title, _width, _height, false) {

    fbo = 0;
    fb_tex = 0;
    fb_depth = 0;

    this->createFBO();
} // namespace Chimera

CanvasHmd::~CanvasHmd() {

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &fb_tex);
    glDeleteRenderbuffers(1, &fb_depth);

    if (context != nullptr) {
        SDL_GL_DeleteContext(context);
        context = nullptr;
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void CanvasHmd::before() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void CanvasHmd::after() { SDL_GL_SwapWindow(window); }

void CanvasHmd::reshape(int _width, int _height) {
    width = _width;
    height = _height;
}

void CanvasHmd::toggleFullScreen() {

    if (fullScreen == false) {

        SDL_GetWindowPosition(window, &winPosPrev.x, &winPosPrev.y);

        SDL_SetWindowPosition(window, 0, 0);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    } else {

        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowPosition(window, winPosPrev.x, winPosPrev.y);
    }

    fullScreen = !fullScreen;
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

glm::mat4 CanvasHmd::getOrthoProjectionMatrix(int eyeIndex) {

    return glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
}

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

void CanvasHmd::initGL() {

    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw Exception(std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw Exception(std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }

    // Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw Exception(std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }
}

void CanvasHmd::afterStart() {

    // glEnable ( GL_TEXTURE_2D );
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // glEnable ( GL_LIGHTING );
}

void CanvasHmd::restoreMatrix() {

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
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

void CanvasHmd::createFBO() {

    // ref: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

    if (!fbo) {
        // pass 1 cria framebuffer// GLuint FramebufferName = 0; => fbo
        glGenFramebuffers(1, &fbo);
        // pass 2 textura a ser usada // GLuint renderedTexture => fb_tex
        glGenTextures(1, &fb_tex);
        // pass 3  // The depth buffer //GLuint depthrenderbuffer => fb_depth
        glGenRenderbuffers(1, &fb_depth);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, fb_tex);

        // Filtro linear
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    fbTexSize.w = next_pow2(width);
    fbTexSize.h = next_pow2(height);

    /* create and attach the texture that will be used as a color buffer */
    glBindTexture(GL_TEXTURE_2D, fb_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbTexSize.w, fbTexSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbTexSize.w, fbTexSize.h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);

    // Pass 4
    // Set "renderedTexture" as our colour attachement #0
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Set the list of draw buffers.
    // GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    // glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw Exception(std::string("Falha em instanciar o Frame Buffer"));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    printf("created render target: %dx%d (texture size: %dx%d)\n", width, height, fbTexSize.w, fbTexSize.h);
}

} // namespace Chimera
