#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/ShadersLoader.hpp"

namespace Chimera {

CanvasHmd::CanvasHmd(const std::string& _title, int _width, int _height)
    : CanvasGL(_title, _width * 2, _height, false) {

    Chimera::ShadersLoader sl;
    pShader = sl.loadShader("RenderToTex", "./libs/chimera/shader/Passthrough.vertexshader",
                            "./libs/chimera/shader/WobblyTexture.fragmentshader");

    pLeft = new Eye(0, _width, _height, pShader);
    // pRight = new Eye(1, _width, _height, pShader);

} // namespace Chimera

CanvasHmd::~CanvasHmd() {
    delete pLeft;
    // delete pRight;
    delete pShader;
}

void CanvasHmd::before() {
    pLeft->begin();     // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    CanvasGL::before(); // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CanvasHmd::after() {

    pLeft->end();            // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    pLeft->displayTexture(); // atual
    CanvasGL::after();       // SDL_GL_SwapWindow(window);
}

glm::mat4 CanvasHmd::getPerspectiveProjectionMatrix(ViewPoint* vp, int _eye) {
    // TODO: user o trackhead para posicao dos olhos
    return pLeft->getPerspectiveProjectionMatrix(vp);
}

glm::mat4 CanvasHmd::getOrthoProjectionMatrix(int eyeIndex) { return pLeft->getOrthoProjectionMatrix(); }

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
