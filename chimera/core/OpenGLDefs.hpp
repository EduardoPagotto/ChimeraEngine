#pragma once
#include <GL/glew.h>

#include <GL/gl.h>

#include <glm/glm.hpp>

#define EPSILON 1e-2 // FIXME: para 1e-1 Falhas em calculos de aproximação verificar projeto bsptree

#define IS_NEAR(v1, v2) fabs(v1 - v2) < EPSILON
#define IS_NEARVEC3(v1, v2) IS_NEAR(v1.x, v2.x) && IS_NEAR(v1.y, v2.y) && IS_NEAR(v1.z, v2.z)
#define IS_NEARVEC2(v1, v2) IS_NEAR(v1.x, v2.x) && IS_NEAR(v1.y, v2.y)
#define GETSIZEMINMAX(min, max) glm::vec3(glm::abs(max.x - min.x), glm::abs(max.y - min.y), glm::abs(max.z - min.z));

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
} // namespace Chimera