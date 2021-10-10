#include "chimera/core/windows/CanvasHmd.hpp"

namespace Chimera {

CanvasHmd::CanvasHmd(const std::string& _title, int _width, int _height) : CanvasGL(_title, _width * 2, _height, false) {
    pShader = new Shader("./assets/shaders/CanvasHMD.glsl");
    pLeft = new Eye(0, _width, _height, pShader);
    pRight = new Eye(1, _width, _height, pShader);
}

CanvasHmd::~CanvasHmd() {
    delete pLeft;
    delete pRight;
    delete pShader;
}

void CanvasHmd::before(const unsigned short& _indexEye) {

    if (_indexEye == 0)
        pLeft->bind();
    else
        pRight->bind();

    CanvasGL::before(_indexEye);
}

void CanvasHmd::swapWindow() {

    CanvasGL::before();
    pLeft->displayTexture();
    pRight->displayTexture();
    CanvasGL::swapWindow();
}

void CanvasHmd::after(const unsigned short& _indexEye) {

    if (_indexEye == 0) {
        pLeft->unbind();
    } else {
        pRight->unbind();
    }
    // CanvasGL::after(); // SDL_GL_SwapWindow(window);
}
} // namespace Chimera
