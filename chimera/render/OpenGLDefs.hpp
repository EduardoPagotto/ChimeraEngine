#pragma once

#include <GL/glew.h>

#include <GL/gl.h>
//#include <GL/glu.h>
// FIXME: colocar o namespace
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

// class GLCache {
//   public:
//     void savePixelState() {
//         // glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
//         // glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
//         // glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
//         // glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
//         // glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
//         // glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

//         glGetIntegerv(GL_PACK_SWAP_BYTES, &swapbytes);
//         glGetIntegerv(GL_PACK_LSB_FIRST, &lsbfirst);
//         glGetIntegerv(GL_PACK_ROW_LENGTH, &rowlength);
//         glGetIntegerv(GL_PACK_SKIP_ROWS, &skiprows);
//         glGetIntegerv(GL_PACK_SKIP_PIXELS, &skippixels);
//         glGetIntegerv(GL_PACK_ALIGNMENT, &alignment);
//     }

//     void restorePixelState() {
//         // glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
//         // glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
//         // glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
//         // glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
//         // glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
//         // glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

//         glPixelStorei(GL_PACK_SWAP_BYTES, swapbytes);
//         glPixelStorei(GL_PACK_LSB_FIRST, lsbfirst);
//         glPixelStorei(GL_PACK_ROW_LENGTH, rowlength);
//         glPixelStorei(GL_PACK_SKIP_ROWS, skiprows);
//         glPixelStorei(GL_PACK_SKIP_PIXELS, skippixels);
//         glPixelStorei(GL_PACK_ALIGNMENT, alignment);
//     }

//   public:
//     GLint swapbytes, lsbfirst, rowlength, skiprows, skippixels, alignment;
// };
