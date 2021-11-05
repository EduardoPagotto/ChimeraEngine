#pragma once

#include "CanvasGL.hpp"
#include "chimera/render/buffers/RenderBuffer.hpp"

namespace Chimera {

class CanvasHmd : public CanvasGL {
  public:
    CanvasHmd(const std::string& _title, int _width, int _height);
    virtual ~CanvasHmd();

    virtual void before(const unsigned short& _indexEye = 0);
    virtual void after(const unsigned short& _indexEye = 0);
    virtual void swapWindow();
    virtual int getTotEyes() { return 2; }
    RenderBuffer* getEye(const unsigned short& _indexEye = 0) { return (_indexEye == 0) ? pLeft : pRight; }

  private:
    RenderBuffer* pLeft;
    RenderBuffer* pRight;
    Shader shader;
};
} // namespace Chimera

#endif