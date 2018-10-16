//#include <GL/glext.h>
//#include "OpenGLDefs.h"

#include <vector>

#include "HmdDevice.h"
#include "ExceptionSDL.h"

#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

// Referencia:
// https://stackoverflow.com/questions/12157646/how-to-render-offscreen-on-opengl
// https://www.gamedev.net/forums/topic/571627-how-to-render-32-bit-integer-into-textures/

namespace Chimera {

HmdDevice::HmdDevice ( std::string _nome ) : Video ( _nome, KIND_DEVICE::HMD_Z1 ) {

    winSizeW = 800;
    winSizeH = 480;
    initSDL();
    initDevice();
}

HmdDevice::HmdDevice ( int _width, int _height, std::string _nome ) : Video ( _nome, KIND_DEVICE::SCREEN, _width, _height ) {

    // FIXME: necessario verificar se nao é outyro valor como o do olho +- 960x540 (1920x1080 / 2 ou 900x900)
    winSizeW = _width; // winSizeW/2
    winSizeH = _height;

    //chama inicializacao do SDL na classe Pai
    initSDL();
    initDevice();
}

HmdDevice::~HmdDevice() {
}

void HmdDevice::initDevice(void){


    glGenFramebuffers(1,&fbo);
    glGenRenderbuffers(1,&render_buf);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, winSizeW, winSizeH); //GL_RGBA ??
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render_buf);
}

void HmdDevice::initDraw() {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

}

void HmdDevice::endDraw() {

    std::vector<std::uint8_t> data( winSizeW * winSizeH * 4);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, winSizeW ,winSizeH, GL_RGBA8, GL_UNSIGNED_BYTE, &data[0]); //GL_RGBA ??
    // Return to onscreen rendering:
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    SDL_GL_SwapWindow ( window );
}

glm::mat4 HmdDevice::getPerspectiveProjectionMatrix(const float &_fov, const float &_near, const float &_far, int _eye) {
//void VideoDevice::executeViewPerspective ( const float &_fov,const float &_near,const float &_far, int _eye ) {

    glViewport ( 0, 0, winSizeW, winSizeH );
	return glm::perspective(_fov, (GLfloat)(float)winSizeW / (float)winSizeH, _near, _far);

    //glMatrixMode ( GL_PROJECTION );
    //glLoadIdentity();
    //gluPerspective ( _fov, ( GLfloat ) ( float ) winSizeW / ( float ) winSizeH, _near, _far );
    //glMatrixMode ( GL_MODELVIEW );
    //glLoadIdentity();
}

//void HmdDevice::perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )//TODO subistituir o executeViewPerspective
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

glm::mat4 HmdDevice::getOrthoProjectionMatrix( int eyeIndex ) {
 
    return glm::ortho(0.0f, static_cast<GLfloat>(winSizeW), 0.0f, static_cast<GLfloat>(winSizeH));
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

void HmdDevice::reshape ( int _w, int _h ) {
    winSizeW = _w;
    winSizeH = _h;
}

void HmdDevice::toggleFullScreen() {

    // if ( fullscreenStatus == false ) {

    //     SDL_GetWindowPosition ( window, &winPosPrev.x, &winPosPrev.y );

    //     SDL_SetWindowPosition ( window, 0, 0 );
    //     SDL_SetWindowFullscreen ( window, SDL_WINDOW_FULLSCREEN_DESKTOP );

    // } else {

    //     SDL_SetWindowFullscreen ( window, 0 );
    //     SDL_SetWindowPosition ( window, winPosPrev.x, winPosPrev.y );

    // }

    // fullscreenStatus = !fullscreenStatus;
}
}
