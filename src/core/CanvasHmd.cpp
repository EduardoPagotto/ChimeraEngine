#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/ShadersLoader.hpp"

namespace Chimera {

CanvasHmd::CanvasHmd(const std::string& _title, int _width, int _height)
    : CanvasGL(_title, _width * 2, _height, false) {

    Chimera::ShadersLoader sl;
    pShader = sl.loadShader("RenderToTex", "./libs/chimera/shader/Passthrough.vertexshader",
                            "./libs/chimera/shader/WobblyTexture.fragmentshader");

    pLeft = new Eye(0, _width, _height, pShader);
    pRight = new Eye(1, _width, _height, pShader);

    this->log = Chimera::Logger::get();

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

void CanvasHmd::calcPerspectiveProjectionView(int _eye, ViewPoint* vp, glm::mat4& view, glm::mat4& projection) {

    if (_eye == 0) {
        // glm::vec3 right = glm::cross(vp->front, vp->worldUp); // glm::normalize(glm::cross(vp->front, vp->worldUp));
        // log->debug("X: %.3f ; Y: %.3f ; Z: %.3f", right.x, right.y, right.z);
        ViewPoint nova = ViewPoint(*vp);
        nova.position.x = 50;
        // vp->position += right.x + 3.0f;
        projection = pLeft->getPerspectiveProjectionMatrix(&nova);
        // View Matrix
        view = glm::lookAt(nova.position, nova.front, nova.up);
    } else {
        projection = pRight->getPerspectiveProjectionMatrix(vp);
        view = glm::lookAt(vp->position, vp->front, vp->up);
        // CanvasGL::calcPerspectiveProjectionView(_eye, vp, view, projection);
    }
}

glm::mat4 CanvasHmd::getPerspectiveProjectionMatrix(ViewPoint* vp, int _eye) {
    // TODO: user o trackhead para posicao dos olhos
    if (_eye == 0) {
        // glm::vec3 right = glm::cross(vp->front, vp->worldUp); // glm::normalize(glm::cross(vp->front, vp->worldUp));
        // log->debug("X: %.3f ; Y: %.3f ; Z: %.3f", right.x, right.y, right.z);
        ViewPoint nova = ViewPoint(*vp);

        nova.position.x = 500;

        // vp->position += right.x + 3.0f;
        return pLeft->getPerspectiveProjectionMatrix(&nova);
    }

    return pRight->getPerspectiveProjectionMatrix(vp);
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
