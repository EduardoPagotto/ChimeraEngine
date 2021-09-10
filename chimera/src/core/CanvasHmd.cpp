#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/utils.hpp"

namespace Chimera {

CanvasHmd::CanvasHmd(const std::string& _title, int _width, int _height) : CanvasGL(_title, _width * 2, _height, false) {

    pShader = new Shader("CanvasHMD", "./chimera/shaders/CanvasHMD.vert", "./chimera/shaders/CanvasHMD.frag");

    pLeft = new Eye(0, _width, _height, pShader);
    pRight = new Eye(1, _width, _height, pShader);
} // namespace Chimera

CanvasHmd::~CanvasHmd() {
    delete pLeft;
    delete pRight;
    delete pShader;
}

void CanvasHmd::before(const unsigned short& _indexEye) {

    if (_indexEye == 0)
        pLeft->begin(); // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    else
        pRight->begin();

    CanvasGL::before(_indexEye); // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CanvasHmd::swapWindow() {

    CanvasGL::before();

    pLeft->displayTexture();
    pRight->displayTexture();
    CanvasGL::swapWindow();
}

void CanvasHmd::after(const unsigned short& _indexEye) {

    if (_indexEye == 0) {
        pLeft->end(); // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        pRight->end(); // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // CanvasGL::after(); // SDL_GL_SwapWindow(window);
}

void CanvasHmd::calcPerspectiveProjectionView(const unsigned short& _indexEye, ViewPoint* vp, glm::mat4& view, glm::mat4& projection) {

    if (_indexEye == 0) {

        projection = pLeft->getPerspectiveProjectionMatrix(vp);
        view = glm::lookAt(vp->position, vp->front, vp->up);

    } else {

        ViewPoint nova = ViewPoint(*vp);

        glm::vec3 left_p = nova.front - nova.position;
        glm::vec3 cross1 = glm::cross(nova.up, left_p);
        glm::vec3 norm1 = glm::normalize(cross1);

        glm::vec3 final_norm1 = norm1 * 50.0f;

        nova.position = vp->position - final_norm1;
        nova.front = vp->front - final_norm1;

        projection = pRight->getPerspectiveProjectionMatrix(&nova);
        view = glm::lookAt(nova.position, nova.front, nova.up);
    }

    // CanvasGL::calcPerspectiveProjectionView(_eye, vp, view, projection);
}

glm::mat4 CanvasHmd::getOrthoProjectionMatrix(int eyeIndex) {
    if (eyeIndex == 0)
        return pLeft->getOrthoProjectionMatrix();

    return pRight->getOrthoProjectionMatrix();
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
