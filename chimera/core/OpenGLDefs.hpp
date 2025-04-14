#pragma once
#include <GL/glew.h>

#include <GL/gl.h>

#include <glm/glm.hpp>

namespace ce {

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
    BinaryStateEnable(const GLenum& attr, const GLboolean& newState) : changed(false), attr(attr) {
        glGetBooleanv(attr, &oldState);
        if (newState != oldState) {
            if (newState == GL_TRUE)
                glEnable(attr);
            else
                glDisable(attr);

            changed = true;
        }
    }

    ~BinaryStateEnable() {
        if (changed == true) {
            if (oldState == GL_TRUE)
                glEnable(attr);
            else
                glDisable(attr);
        }
    }

  private:
    bool changed;
    GLboolean oldState;
    GLint attr;
};
} // namespace ce