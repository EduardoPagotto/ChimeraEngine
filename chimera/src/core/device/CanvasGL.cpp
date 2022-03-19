#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

CanvasGL::CanvasGL(const std::string& _title, int _width, int _height, bool _fullScreen) : Canvas(_title, _width, _height, _fullScreen) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::string("Falha SDL_Init:" + std::string(SDL_GetError()));
    }

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

        throw std::string("Falha Criar Janela SDL:" + std::string(SDL_GetError()));
    }

    // Contexto do SDL do GL
    if ((context = SDL_GL_CreateContext(window)) == nullptr) {
        throw std::string("Falha Criar contexto SDL:" + std::string(SDL_GetError()));
    }

    // Swap buffer interval
    int interval = SDL_GL_SetSwapInterval(1);
    if (interval < 0) {
        throw std::string("Falha ao Ajustar o VSync:" + std::string(SDL_GetError()));
    }

    // SDL_GetWindowPosition(window, &winGeometry.x, &winGeometry.y);
    SDL_GetWindowSize(window, &width, &height);

    // TODO: Testar
    // SDL_GetWindowPosition(window, &posX, &posY);

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

void CanvasGL::before(const unsigned short& _indexEye) {
    // FIXME: remover quando ter coragem de refazer o scene completo :( )
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CanvasGL::after(const unsigned short& _indexEye) {

    GLenum erro = glGetError();
    if (erro != GL_NO_ERROR) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL Erro: %d", erro);
    }
}

void CanvasGL::swapWindow() { SDL_GL_SwapWindow(window); }

void CanvasGL::reshape(int _width, int _height) {
    width = _width;
    height = _height;
}

void CanvasGL::toggleFullScreen() {

    if (fullScreen == false) {

        SDL_GetWindowPosition(window, &posX, &posY);

        SDL_SetWindowPosition(window, 0, 0);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    } else {

        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowPosition(window, posX, posY);
    }

    fullScreen = !fullScreen;
}

std::string CanvasGL::getVersaoOpenGL() {

    std::string retorno = "";
    const char* version = (const char*)glGetString(GL_VERSION);

    if (version != nullptr) {
        retorno.append(version);
    } else {
        // Check for error
        GLenum error = glGetError();
        throw std::string((const char*)gluErrorString(error));
    }

    return retorno;
}

} // namespace Chimera
