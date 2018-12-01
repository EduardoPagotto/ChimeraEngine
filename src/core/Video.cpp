#include "chimera/core/Video.hpp"

#include "chimera/core/ExceptionChimera.hpp"

namespace Chimera {

Video::Video(std::string _nome, KIND_DEVICE _kindDevice) : nomeTela(_nome), kindDevice(_kindDevice) {

    winSizeW = 800;
    winSizeH = 600;

    log = spdlog::get("chimera");
    log->debug("Contructor Video:{0} w:{1:03d} h:{2:03d}", _nome, winSizeW, winSizeH);
}

Video::Video(std::string _nome, KIND_DEVICE _kindDevice, int _w, int _h) : nomeTela(_nome), kindDevice(_kindDevice) {

    winSizeW = _w;
    winSizeH = _h;

    log = spdlog::get("chimera");
    log->debug("Contructor Video:{0} w:{1:03d} h:{2:03d}", _nome, winSizeW, winSizeH);
}

Video::~Video() {

    if (context != nullptr) {
        SDL_GL_DeleteContext(context);
        context = nullptr;
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    log->debug("Destructor Video");

    SDL_Quit();
}

void Video::initSDL() {

    // Inicializa o SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw ExceptionChimera(std::string(std::string("Falha init SDL:") + SDL_GetError()));
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
    if ((window = SDL_CreateWindow(nomeTela.c_str(),        // window title
                                   SDL_WINDOWPOS_UNDEFINED, // initial x position
                                   SDL_WINDOWPOS_UNDEFINED, // initial y position
                                   winSizeW,                // width, in pixels
                                   winSizeH,                // height, in pixels
                                   flags)) == nullptr) {

        throw ExceptionChimera(std::string(std::string("Falha Criar Janela SDL:") + SDL_GetError()));
    }

    // Contexto do SDL
    if ((context = SDL_GL_CreateContext(window)) == nullptr) {
        throw ExceptionChimera(std::string(std::string("Falha Criar contexto SDL:") + SDL_GetError()));
    }

    // Swap buffer interval
    int interval = SDL_GL_SetSwapInterval(1);
    if (interval < 0) {
        throw ExceptionChimera(std::string("Falha ao Ajustar o VSync:" + std::string(SDL_GetError())));
    }

    // SDL_GetWindowPosition(window, &winGeometry.x, &winGeometry.y);
    SDL_GetWindowSize(window, &winSizeW, &winSizeH);

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

void Video::initGL() {

    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw ExceptionChimera(
            std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw ExceptionChimera(
            std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }

    // Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Check for error
    error = glGetError();

    if (error != GL_NO_ERROR) {
        throw ExceptionChimera(
            std::string("Falha ao Iniciar o OpenGL:" + std::string((const char*)gluErrorString(error))));
    }
}

void Video::afterStart() {

    // glEnable ( GL_TEXTURE_2D );
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // glEnable ( GL_LIGHTING );
}

void Video::restoreMatrix() {

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

std::string Video::getVersaoOpenGL() {

    std::string retorno = "";
    const char* version = (const char*)glGetString(GL_VERSION);

    if (version != nullptr) {
        retorno.append(version);
    } else {
        // Check for error
        GLenum error = glGetError();
        throw ExceptionChimera(std::string((const char*)gluErrorString(error)));
    }

    return retorno;
}

} // namespace Chimera