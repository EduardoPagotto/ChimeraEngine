#pragma once

#include <GL/glew.h>

#include <GL/gl.h>
//#include <GL/glu.h>

namespace Chimera {

class DepthFuncSetter {
  public:
    DepthFuncSetter(GLenum f) : changed(false) {
        glGetIntegerv(GL_DEPTH_FUNC, &oldSetting);
        if (f != oldSetting) {
            glDepthFunc(f);
            changed = true;
        }
    }

    ~DepthFuncSetter() {
        if (changed)
            glDepthFunc(oldSetting);
    }

  private:
    GLint oldSetting;
    bool changed;
};

class BinaryStateEnable {
  public:
    BinaryStateEnable(GLenum attr) : changed(false), attr(attr) {
        GLboolean oldVal;
        glGetBooleanv(attr, &oldVal);
        if (oldVal != GL_TRUE) {
            changed = true;
            glEnable(attr);
        }
    }

    ~BinaryStateEnable() {
        if (changed == true)
            glDisable(attr);
    }

  private:
    bool changed;
    GLint attr;
};

class BinaryStateDisable {
  public:
    BinaryStateDisable(GLenum attr) : changed(false), attr(attr) {
        GLboolean oldVal;
        glGetBooleanv(attr, &oldVal);
        if (oldVal != GL_FALSE) {
            changed = true;
            glDisable(attr);
        }
    }

    ~BinaryStateDisable() {
        if (changed == true)
            glEnable(attr);
    }

  private:
    bool changed;
    GLint attr;
};
} // namespace Chimera