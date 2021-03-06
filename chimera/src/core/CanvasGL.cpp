#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CanvasGL::CanvasGL(const std::string& _title, int _width, int _height, bool _fullScreen)
    : Canvas(_title, _width, _height, _fullScreen) {

    // Ajusta o contexto de versao do opengl
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // turn on double buffering set the depth buffer to 24 bits
    // you may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Cria aJanela
    Uint32 flags = SDL_WINDOW_OPENGL;                       // | SDL_WINDOW_SHOWN;
    if ((window = SDL_CreateWindow(title.c_str(),           // window title
                                   SDL_WINDOWPOS_UNDEFINED, // initial x position
                                   SDL_WINDOWPOS_UNDEFINED, // initial y position
                                   width,                   // width, in pixels
                                   height,                  // height, in pixels
                                   flags)) == nullptr) {

        throw Exception(std::string(std::string("Falha Criar Janela SDL:") + SDL_GetError()));
    }

    // Contexto do SDL do GL
    if ((context = SDL_GL_CreateContext(window)) == nullptr) {
        throw Exception(std::string(std::string("Falha Criar contexto SDL:") + SDL_GetError()));
    }

    // Swap buffer interval
    int interval = SDL_GL_SetSwapInterval(1);
    if (interval < 0) {
        throw Exception(std::string("Falha ao Ajustar o VSync:" + std::string(SDL_GetError())));
    }

    // SDL_GetWindowPosition(window, &winGeometry.x, &winGeometry.y);
    SDL_GetWindowSize(window, &width, &height);

    // TODO: Testar
    // SDL_GetWindowPosition(window, &winPosPrev.x, &winPosPrev.y);

    // iniciala GLEW
    glewExperimental = GL_TRUE;
    glewInit();

#ifdef WIN32
    // Here we initialize our multi-texturing functions
    glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
    glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

    // Make sure our multi-texturing extensions were loaded correctly
    if (!glActiveTextureARB || !glMultiTexCoord2fARB) {
        throw ExceptionSDL(ExceptionCode::ALLOC, std::string("Your current setup does not support multitexturing"));
    }
#endif
}

CanvasGL::~CanvasGL() {
    if (context != nullptr) {
        SDL_GL_DeleteContext(context);
        context = nullptr;
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void CanvasGL::before(const unsigned short& _indexEye) { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void CanvasGL::after(const unsigned short& _indexEye) {}

void CanvasGL::swapWindow() { SDL_GL_SwapWindow(window); }

void CanvasGL::reshape(int _width, int _height) {
    width = _width;
    height = _height;
}

void CanvasGL::toggleFullScreen() {

    if (fullScreen == false) {

        SDL_GetWindowPosition(window, &winPosPrev.x, &winPosPrev.y);

        SDL_SetWindowPosition(window, 0, 0);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    } else {

        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowPosition(window, winPosPrev.x, winPosPrev.y);
    }

    fullScreen = !fullScreen;
}

void CanvasGL::calcPerspectiveProjectionView(const unsigned short& _indexEye, ViewPoint* vp, glm::mat4& view,
                                             glm::mat4& projection) {
    glViewport(0, 0, width, height);
    projection = glm::perspective(vp->fov, (GLfloat)(float)width / (float)height, vp->near, vp->far);
    view = glm::lookAt(vp->position, vp->front, vp->up);
}

glm::mat4 CanvasGL::getOrthoProjectionMatrix(int eyeIndex) {
    return glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
}

void CanvasGL::initGL() {

    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw Exception(std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw Exception(std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }

    // Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw Exception(std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }
}

void CanvasGL::afterStart() {

    // glEnable ( GL_TEXTURE_2D );
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // glEnable ( GL_LIGHTING );
}

void CanvasGL::restoreMatrix() {

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

std::string CanvasGL::getVersaoOpenGL() {

    std::string retorno = "";
    const char* version = (const char*)glGetString(GL_VERSION);

    if (version != nullptr) {
        retorno.append(version);
    } else {
        // Check for error
        GLenum error = glGetError();
        throw Exception(std::string((const char*)gluErrorString(error)));
    }

    return retorno;
}

} // namespace Chimera
