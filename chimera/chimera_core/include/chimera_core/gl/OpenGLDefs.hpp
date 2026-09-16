#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// ============================================================================
// 1. TODAS AS DEFINIÇÕES DE TIPOS E CONSTANTES EXIGIDAS PELO OPENGL (MANUAL)
// ============================================================================
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef char GLchar;
typedef float GLfloat;
typedef unsigned char GLboolean;
typedef uint64_t GLuint64;
typedef unsigned char GLubyte;
typedef double GLdouble;
typedef float GLclampf;

#define GL_TRUE                        1
#define GL_FALSE                       0
#define GL_NONE                        0
#define GL_NO_ERROR                    0
#define GL_LINES                       0x0001
#define GL_TRIANGLES                   0x0004
#define GL_TRIANGLE_STRIP              0x0005
#define GL_UNSIGNED_BYTE               0x1401
#define GL_UNSIGNED_SHORT              0x1403
#define GL_INT                         0x1404
#define GL_UNSIGNED_INT                0x1405
#define GL_FLOAT                       0x1406
#define GL_LESS                        0x0201
#define GL_LEQUAL                      0x0203
#define GL_NEAREST                     0x2600
#define GL_FRAGMENT_SHADER             0x8B30
#define GL_VERTEX_SHADER               0x8B31
#define GL_COMPILE_STATUS              0x8B81
#define GL_LINK_STATUS                 0x8B82
#define GL_INFO_LOG_LENGTH             0x8B84
#define GL_COLOR_BUFFER_BIT            0x00004000
#define GL_DEPTH_BUFFER_BIT            0x00000100
#define GL_RENDERER                    0x1F01
#define GL_VERSION                     0x1F02
#define GL_ARRAY_BUFFER                0x8892
#define GL_ELEMENT_ARRAY_BUFFER        0x8893
#define GL_WRITE_ONLY                  0x88B9
#define GL_STREAM_DRAW                 0x88E0
#define GL_STATIC_DRAW                 0x88E4
#define GL_DYNAMIC_DRAW                0x88E8
#define GL_DEPTH24_STENCIL8            0x88F0
#define GL_MIRRORED_REPEAT             0x8370
#define GL_CLAMP_TO_EDGE               0x812F
#define GL_CLAMP_TO_BORDER             0x812D
#define GL_DEPTH_COMPONENT             0x1902
#define GL_RGB                         0x1907
#define GL_LUMINANCE                   0x1909
#define GL_RGBA                        0x1908
#define GL_LUMINANCE_ALPHA             0x190A
#define GL_DEPTH_ATTACHMENT            0x8D00
#define GL_FRAMEBUFFER                 0x8D40
#define GL_RENDERBUFFER                0x8D41
#define GL_RED_INTEGER                 0x8D94
#define GL_RGB8                        0x8051
#define GL_RGBA8                       0x8058
#define GL_TEXTURE_WRAP_R              0x8072
#define GL_R32I                        0x8235
#define GL_TEXTURE_2D                  0x0DE1
#define GL_TEXTURE0                    0x84C0
#define GL_TEXTURE_MAG_FILTER          0x2800
#define GL_TEXTURE_MIN_FILTER          0x2801
#define GL_TEXTURE_WRAP_S              0x2802
#define GL_TEXTURE_WRAP_T              0x2803
#define GL_LINEAR                      0x2601
#define GL_LINEAR_MIPMAP_LINEAR        0x2703
#define GL_CLAMP_TO_EDGE               0x812F
#define GL_CLAMP_TO_BORDER             0x812D
#define GL_REPEAT                      0x2901
#define GL_COLOR_ATTACHMENT0           0x8CE0
#define GL_COLOR_ATTACHMENT1           0x8CE1
#define GL_COLOR_ATTACHMENT2           0x8CE2
#define GL_COLOR_ATTACHMENT3           0x8CE3
#define GL_TEXTURE_BORDER_COLOR        0x1004
#define GL_FRAMEBUFFER_COMPLETE        0x8CD5
#define GL_CULL_FACE                   0x0B44
#define GL_DEPTH_TEST                  0x0B71
#define GL_DEPTH_FUNC                  0x0B74
#define GL_BLEND                       0x0BE2
#define GL_SRC_ALPHA                   0x0302
#define GL_ONE_MINUS_SRC_ALPHA         0x0303
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_NICEST                      0x1102
#define GL_CONTEXT_FLAGS               0x821E
#define GL_CONTEXT_FLAG_DEBUG_BIT      0x00000002
#define GL_DEBUG_OUTPUT                0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS    0x8242
#define GL_DEBUG_SEVERITY_HIGH         0x9146
#define GL_DEBUG_SEVERITY_MEDIUM       0x9147
#define GL_DEBUG_SEVERITY_LOW          0x9148
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_VENDOR                      0x1F00
#define GL_RENDERER                    0x1F01
#define GL_VERSION                     0x1F02
#define APIENTRY

// ============================================================================
// 2. ASSINATURAS DE FUNÇÕES DO OPENGL UTILIZADAS NO PIPELINE
// ============================================================================
typedef void (*PFNGLCLEARPROC)(GLbitfield mask);
typedef void (*PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef GLuint (*PFNGLCREATESHADERPROC)(GLenum type);
typedef void (*PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (*PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (*PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void (*PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef GLuint (*PFNGLCREATEPROGRAMPROC)(void);
typedef void (*PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (*PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (*PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void (*PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (*PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (*PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef void (*PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (*PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void (*PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (*PFNGLBUFFERDATAPROC)(GLenum target, ptrdiff_t size, const void* data, GLenum usage);
typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized,
                                             GLsizei stride, const void* pointer);
typedef void (*PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void* indices);
typedef void (*PFNGLENABLEPROC)(GLenum cap);
typedef GLint (*PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void (*PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (*PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (*PFNGLGENTEXTURESPROC)(GLsizei n, GLuint* textures);
typedef void (*PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (*PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (*PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (*PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
                                    GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (*PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef const unsigned char* (*PFNGLGETSTRINGPROC)(GLenum name);
typedef void (*PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
typedef GLboolean (*PFNGLUNMAPBUFFERPROC)(GLenum target);
typedef void (*PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint* textures);
typedef void (*PFNGLBUFFERSUBDATAPROC)(GLenum target, ptrdiff_t offset, ptrdiff_t size, const void* data);
typedef void* (*PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
typedef GLenum (*PFNGLGETERRORPROC)(void);
typedef void (*PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (*PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint* framebuffers);
typedef void (*PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint* framebuffers);
typedef void (*PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum* bufs);
typedef void (*PFNGLDRAWBUFFERPROC)(GLenum buf);
typedef void (*PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint* renderbuffers);
typedef void (*PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);
typedef void (*PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef void (*PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint* renderbuffers);
typedef void (*PFNGLFRAMEBUFFERTEXTUREPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (*PFNGLREADBUFFERPROC)(GLenum mode);
typedef void (*PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
typedef void (*PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (*PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget,
                                                 GLuint renderbuffer);
typedef GLenum (*PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void (*PFNGLUNIFORM1IVPROC)(GLint location, GLsizei count, const GLint* value);
typedef void (*PFNGLUNIFORM2IVPROC)(GLint location, GLsizei count, const GLint* value);
typedef void (*PFNGLUNIFORM3IVPROC)(GLint location, GLsizei count, const GLint* value);
typedef void (*PFNGLUNIFORM4IVPROC)(GLint location, GLsizei count, const GLint* value);
typedef void (*PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (*PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (*PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (*PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (*PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (*PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (*PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (*PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (*PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type,
                                    void* pixels);
typedef void (*PFNGLCLEARTEXIMAGEPROC)(GLuint texture, GLint level, GLenum format, GLenum type, const void* data);
typedef void (*PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture,
                                              GLint level);
typedef void (*PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat* params);
typedef void (*PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void (*PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean* data);
typedef void (*PFNGLDISABLEPROC)(GLenum cap);
typedef void (*PFNGLGETINTEGERVPROC)(GLenum pname, GLint* data);
typedef void (*PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (*PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
typedef void (*PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (*PFNGLCLEARDEPTHPROC)(GLdouble depth);
typedef void (*PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void (*PFNGLHINTPROC)(GLenum target, GLenum mode);

// DEBUG
typedef void(APIENTRY* GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                    const GLchar* message, const void* userParam);
typedef void(APIENTRY* PFNGLDEBUGMESSAGECALLBACKPROC)(GLDEBUGPROC callback, const void* userParam);
//  --
inline PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
inline PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
inline PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
inline PFNGLREADBUFFERPROC glReadBuffer;
inline PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
inline PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
inline PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
inline PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
inline PFNGLUNIFORM1IVPROC glUniform1iv;
inline PFNGLUNIFORM2IVPROC glUniform2iv;
inline PFNGLUNIFORM3IVPROC glUniform3iv;
inline PFNGLUNIFORM4IVPROC glUniform4iv;
inline PFNGLUNIFORM1FVPROC glUniform1fv;
inline PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
inline PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
inline PFNGLCLEARPROC glClear;
inline PFNGLCLEARCOLORPROC glClearColor;
inline PFNGLCREATESHADERPROC glCreateShader;
inline PFNGLSHADERSOURCEPROC glShaderSource;
inline PFNGLCOMPILESHADERPROC glCompileShader;
inline PFNGLGETSHADERIVPROC glGetShaderiv;
inline PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
inline PFNGLCREATEPROGRAMPROC glCreateProgram;
inline PFNGLATTACHSHADERPROC glAttachShader;
inline PFNGLLINKPROGRAMPROC glLinkProgram;
inline PFNGLGETPROGRAMIVPROC glGetProgramiv;
inline PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
inline PFNGLUSEPROGRAMPROC glUseProgram;
inline PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
inline PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
inline PFNGLGENBUFFERSPROC glGenBuffers;
inline PFNGLBINDBUFFERPROC glBindBuffer;
inline PFNGLBUFFERDATAPROC glBufferData;
inline PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
inline PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
inline PFNGLDRAWELEMENTSPROC glDrawElements;
inline PFNGLENABLEPROC glEnable;
inline PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
inline PFNGLUNIFORM3FVPROC glUniform3fv;
inline PFNGLGENTEXTURESPROC glGenTextures;
inline PFNGLBINDTEXTUREPROC glBindTexture;
inline PFNGLACTIVETEXTUREPROC glActiveTexture;
inline PFNGLTEXPARAMETERIPROC glTexParameteri;
inline PFNGLTEXIMAGE2DPROC glTexImage2D;
inline PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
inline PFNGLGETSTRINGPROC glGetString;
inline PFNGLDELETEBUFFERSPROC glDeleteBuffers;
inline PFNGLUNMAPBUFFERPROC glUnmapBuffer;
inline PFNGLDELETETEXTURESPROC glDeleteTextures;
inline PFNGLBUFFERSUBDATAPROC glBufferSubData;
inline PFNGLMAPBUFFERPROC glMapBuffer;
inline PFNGLGETERRORPROC glGetError;
inline PFNGLDRAWARRAYSPROC glDrawArrays;
inline PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
inline PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
inline PFNGLDRAWBUFFERSPROC glDrawBuffers;
inline PFNGLDRAWBUFFERPROC glDrawBuffer;
inline PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
inline PFNGLVIEWPORTPROC glViewport;
inline PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
inline PFNGLUNIFORM2FVPROC glUniform2fv;
inline PFNGLUNIFORM4FVPROC glUniform4fv;
inline PFNGLDELETEPROGRAMPROC glDeleteProgram;
inline PFNGLDELETESHADERPROC glDeleteShader;
inline PFNGLDETACHSHADERPROC glDetachShader;
inline PFNGLREADPIXELSPROC glReadPixels;
inline PFNGLCLEARTEXIMAGEPROC glClearTexImage;
inline PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
inline PFNGLTEXPARAMETERFVPROC glTexParameterfv;
inline PFNGLDEPTHFUNCPROC glDepthFunc;
inline PFNGLGETBOOLEANVPROC glGetBooleanv;
inline PFNGLDISABLEPROC glDisable;
inline PFNGLGETINTEGERVPROC glGetIntegerv;
inline PFNGLBLENDFUNCPROC glBlendFunc;
inline PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
inline PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
inline PFNGLCLEARDEPTHPROC glClearDepth;
inline PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
inline PFNGLHINTPROC glHint;
inline PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback_ptr;

// Mecanismo de resolução em runtime do SDL3 para carregar o Driver EGL do Wayland
inline void CarregarOpenGL() { // NOLINT
#define LOAD_PROC(type, name)                                                                                          \
    name = reinterpret_cast<type>(SDL_GL_GetProcAddress(#name));                                                       \
    if (!(name)) {                                                                                                     \
        std::cerr << "Falha Crítica EGL/Wayland ao mapear: " << #name << "\n";                                         \
    }

    LOAD_PROC(PFNGLCLEARPROC, glClear);
    LOAD_PROC(PFNGLCLEARCOLORPROC, glClearColor);
    LOAD_PROC(PFNGLCREATESHADERPROC, glCreateShader);
    LOAD_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
    LOAD_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
    LOAD_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
    LOAD_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    LOAD_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    LOAD_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
    LOAD_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
    LOAD_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    LOAD_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    LOAD_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);
    LOAD_PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
    LOAD_PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
    LOAD_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
    LOAD_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
    LOAD_PROC(PFNGLBUFFERDATAPROC, glBufferData);
    LOAD_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    LOAD_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    LOAD_PROC(PFNGLDRAWELEMENTSPROC, glDrawElements);
    LOAD_PROC(PFNGLENABLEPROC, glEnable);
    LOAD_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
    LOAD_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
    LOAD_PROC(PFNGLUNIFORM3FVPROC, glUniform3fv);
    LOAD_PROC(PFNGLGENTEXTURESPROC, glGenTextures);
    LOAD_PROC(PFNGLBINDTEXTUREPROC, glBindTexture);
    LOAD_PROC(PFNGLACTIVETEXTUREPROC, glActiveTexture);
    LOAD_PROC(PFNGLTEXPARAMETERIPROC, glTexParameteri);
    LOAD_PROC(PFNGLTEXIMAGE2DPROC, glTexImage2D);
    LOAD_PROC(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
    LOAD_PROC(PFNGLGETSTRINGPROC, glGetString);
    LOAD_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
    LOAD_PROC(PFNGLUNMAPBUFFERPROC, glUnmapBuffer);
    LOAD_PROC(PFNGLDELETETEXTURESPROC, glDeleteTextures);
    LOAD_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
    LOAD_PROC(PFNGLMAPBUFFERPROC, glMapBuffer);
    LOAD_PROC(PFNGLGETERRORPROC, glGetError);
    LOAD_PROC(PFNGLDRAWARRAYSPROC, glDrawArrays);
    LOAD_PROC(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
    LOAD_PROC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
    LOAD_PROC(PFNGLDRAWBUFFERSPROC, glDrawBuffers);
    LOAD_PROC(PFNGLDRAWBUFFERPROC, glDrawBuffer);
    LOAD_PROC(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers);
    LOAD_PROC(PFNGLVIEWPORTPROC, glViewport);
    LOAD_PROC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
    LOAD_PROC(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
    LOAD_PROC(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers);
    LOAD_PROC(PFNGLFRAMEBUFFERTEXTUREPROC, glFramebufferTexture);
    LOAD_PROC(PFNGLREADBUFFERPROC, glReadBuffer);
    LOAD_PROC(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer);
    LOAD_PROC(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage);
    LOAD_PROC(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer);
    LOAD_PROC(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
    LOAD_PROC(PFNGLUNIFORM1IVPROC, glUniform1iv);
    LOAD_PROC(PFNGLUNIFORM2IVPROC, glUniform2iv);
    LOAD_PROC(PFNGLUNIFORM3IVPROC, glUniform3iv);
    LOAD_PROC(PFNGLUNIFORM4IVPROC, glUniform4iv);
    LOAD_PROC(PFNGLUNIFORM1FVPROC, glUniform1fv);
    LOAD_PROC(PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv);
    LOAD_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
    LOAD_PROC(PFNGLUNIFORM2FVPROC, glUniform2fv);
    LOAD_PROC(PFNGLUNIFORM4FVPROC, glUniform4fv);
    LOAD_PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    LOAD_PROC(PFNGLDELETESHADERPROC, glDeleteShader);
    LOAD_PROC(PFNGLDETACHSHADERPROC, glDetachShader);
    LOAD_PROC(PFNGLREADPIXELSPROC, glReadPixels);
    LOAD_PROC(PFNGLCLEARTEXIMAGEPROC, glClearTexImage);
    LOAD_PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
    LOAD_PROC(PFNGLTEXPARAMETERFVPROC, glTexParameterfv);
    LOAD_PROC(PFNGLDEPTHFUNCPROC, glDepthFunc);
    LOAD_PROC(PFNGLGETBOOLEANVPROC, glGetBooleanv);
    LOAD_PROC(PFNGLDISABLEPROC, glDisable);
    LOAD_PROC(PFNGLGETINTEGERVPROC, glGetIntegerv);
    LOAD_PROC(PFNGLBLENDFUNCPROC, glBlendFunc);
    LOAD_PROC(PFNGLVERTEXATTRIBDIVISORPROC, glVertexAttribDivisor);
    LOAD_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
    LOAD_PROC(PFNGLCLEARDEPTHPROC, glClearDepth);
    LOAD_PROC(PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced);
    LOAD_PROC(PFNGLHINTPROC, glHint);
    LOAD_PROC(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback_ptr);
}

// --- Implementação do Callback de Debug do OpenGL ---
inline void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam) {

    // Ignorar notificações comuns de performance para não poluir o terminal
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

    std::cerr << "[OpenGL Debug] ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cerr << "CRÍTICO: ";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cerr << "AVISO IMPORTANTE: ";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cerr << "AVISO: ";
            break;
    }
    std::cerr << message << " (ID: " << id << ")\n";
}

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
