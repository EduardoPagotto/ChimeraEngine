#include "coreGL/CanvasGL.hpp"
#include "coreGL/OpenGLDefs.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ce {

CanvasGL::CanvasGL(const std::string& title, int width, int height, bool fullScreen)
    : title(title), width(width), height(height), fullScreen(fullScreen), window(nullptr) {

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
    const Uint32 flags = SDL_WINDOW_OPENGL;                       // | SDL_WINDOW_SHOWN;
    if ((this->window = SDL_CreateWindow(title.c_str(),           // window title
                                         SDL_WINDOWPOS_UNDEFINED, // initial x position
                                         SDL_WINDOWPOS_UNDEFINED, // initial y position
                                         width,                   // width, in pixels
                                         height,                  // height, in pixels
                                         flags)) == nullptr) {

        throw std::string("Falha Criar Janela SDL:" + std::string(SDL_GetError()));
    }

    // Contexto do SDL do GL
    if ((this->context = SDL_GL_CreateContext(this->window)) == nullptr) {
        throw std::string("Falha Criar contexto SDL:" + std::string(SDL_GetError()));
    }

    // Swap buffer interval
    if (const int interval = SDL_GL_SetSwapInterval(1); interval < 0) {
        throw std::string("Falha ao Ajustar o VSync:" + std::string(SDL_GetError()));
    }

    // SDL_GetWindowPosition(window, &winGeometry.x, &winGeometry.y);
    SDL_GetWindowSize(this->window, &width, &height);

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
    if (this->context != nullptr) {
        SDL_GL_DeleteContext(this->context);
        this->context = nullptr;
    }

    if (this->window != nullptr) {
        SDL_DestroyWindow(this->window);
        this->window = nullptr;
    }
}

void CanvasGL::before() {
    // FIXME: remover quando ter coragem de refazer o scene completo :( )
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CanvasGL::after() {

    if (const GLenum erro = glGetError(); erro != GL_NO_ERROR)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL Erro: %d", erro);

    SDL_GL_SwapWindow(this->window);
}

void CanvasGL::reshape(int _width, int _height) {
    this->width = _width;
    this->height = _height;
}

void CanvasGL::toggleFullScreen() {

    if (this->fullScreen == false) {

        SDL_GetWindowPosition(this->window, &posX, &posY);

        SDL_SetWindowPosition(this->window, 0, 0);
        SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    } else {

        SDL_SetWindowFullscreen(this->window, 0);
        SDL_SetWindowPosition(this->window, posX, posY);
    }

    fullScreen = !fullScreen;
}

std::string CanvasGL::getVersaoOpenGL() {

    std::string retorno = "";
    if (const char* version = (const char*)glGetString(GL_VERSION); version != nullptr) {
        retorno.append(version);
    } else {
        // Check for error
        const GLenum error = glGetError();
        throw std::string((const char*)gluErrorString(error));
    }

    return retorno;
}

} // namespace ce
