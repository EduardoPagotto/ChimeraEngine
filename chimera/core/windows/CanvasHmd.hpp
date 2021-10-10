#ifndef __CHIMERA_CANVAS_HDM__HPP
#define __CHIMERA_CANVAS_HDM__HPP

#include "CanvasGL.hpp"
#include "Eye.hpp"

namespace Chimera {

class CanvasHmd : public CanvasGL {
  public:
    CanvasHmd(const std::string& _title, int _width, int _height);
    virtual ~CanvasHmd();

    virtual void before(const unsigned short& _indexEye = 0);
    virtual void after(const unsigned short& _indexEye = 0);
    virtual void swapWindow();
    virtual int getTotEyes() { return 2; }
    Eye* getEye(const unsigned short& _indexEye = 0) { return (_indexEye == 0) ? pLeft : pRight; }

  private:
    Eye* pLeft;
    Eye* pRight;
    Shader* pShader;
};
} // namespace Chimera

#endif