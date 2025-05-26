#include "chimera/core/gl/CanvasGL.hpp"
#include "chimera/core/gl/OpenGLDefs.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ce {

    CanvasGL::CanvasGL(const std::string& title, int width, int height, bool fullScreen)
        : title(title), width(width), height(height), fullScreen(fullScreen), window(nullptr) {

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            throw std::string("Falha SDL_Init:" + std::string(SDL_GetError()));
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2); // FIXME: 3 nao funciona!!!!!
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
        if (!window)
            throw std::string("create Window:" + std::string(SDL_GetError()));

        context = SDL_GL_CreateContext(window);
        if (!context)
            throw std::string("create context:" + std::string(SDL_GetError()));

        if (not SDL_GL_MakeCurrent(window, context)) {
            throw std::string("MakeCurrent:" + std::string(SDL_GetError()));
        }

        // Swap buffer interval // FIXME: PRECISO ??
        if (int interval = SDL_GL_SetSwapInterval(1); interval < 0)
            throw std::string("SetSwapInterval:" + std::string(SDL_GetError()));

        // SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowPosition(window, &posX, &posY);

        // iniciala GLEW
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLEW init: %s", glewGetErrorString(err));
            throw std::string("GLEW Init fail");
        }

        SDL_Log("Renderer: %s", glGetString(GL_RENDERER));
        SDL_Log("OpenGL Version: %s", glGetString(GL_VERSION));

        // #ifdef WIN32
        //     // Here we initialize our multi-texturing functions
        //     glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
        //     glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

        //     // Make sure our multi-texturing extensions were loaded correctly
        //     if (!glActiveTextureARB || !glMultiTexCoord2fARB) {
        //         throw ExceptionSDL(ExceptionCode::ALLOC, std::string("Your current setup does not support
        //         multitexturing"));
        //     }
        // #endif
    }

    CanvasGL::~CanvasGL() {
        if (context)
            SDL_GL_DestroyContext(context);

        if (window)
            SDL_DestroyWindow(window);

        context = nullptr;
        window = nullptr;
    }

    void CanvasGL::before() {
        // FIXME: remover quando ter coragem de refazer o scene completo :( )
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void CanvasGL::after() {

        if (GLenum erro = glGetError(); erro != GL_NO_ERROR)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL Erro: %d", erro);

        SDL_GL_SwapWindow(window);
    }

    void CanvasGL::reshape(int _width, int _height) {
        width = _width;
        height = _height;
    }

    void CanvasGL::toggleFullScreen() {

        if (fullScreen == false) {

            SDL_GetWindowPosition(window, &posX, &posY);

            SDL_SetWindowPosition(window, 0, 0);
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

        } else {

            SDL_SetWindowFullscreen(window, 0);
            SDL_SetWindowPosition(window, posX, posY);
        }

        fullScreen = !fullScreen;
    }
} // namespace ce
