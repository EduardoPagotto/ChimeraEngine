#include "chimera/render/CanvasHmd.hpp"

namespace Chimera {

static unsigned int next_pow2(unsigned int x) {
    x -= 1;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

CanvasHmd::CanvasHmd(const std::string& _title, int _width, int _height) : CanvasGL(_title, _width * 2, _height, false) {
    ShaderManager::load("./assets/shaders/CanvasHMD.glsl", shader);

    unsigned int w1 = next_pow2(_width);
    unsigned int h1 = next_pow2(_height);

    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexWrap::NONE, TexDType::UNSIGNED_BYTE)};

    fbSpec.width = w1;
    fbSpec.height = h1;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

    pLeft = new RenderBuffer(0, 0, new FrameBuffer(fbSpec), shader);
    pRight = new RenderBuffer(w1, 0, new FrameBuffer(fbSpec), shader);
}

CanvasHmd::~CanvasHmd() {
    delete pLeft;
    delete pRight;
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
    pLeft->render();
    pRight->render();
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
