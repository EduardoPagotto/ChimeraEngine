#include "chimera_core/gl/CanvasGL.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"
#include <format>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

namespace ce {

    CanvasGL::CanvasGL(const std::string& title, int width, int height, bool fullScreen)
        : title(title), width(width), height(height), fullScreen(fullScreen), window(nullptr) {

        // if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11")) {
        //     throw std::runtime_error("SDL X11 Failed:" + std::string(SDL_GetError()));
        // }
        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland")) {
            throw std::runtime_error(std::format("SDL wayland Failed driver: {}", SDL_GetError()));
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Falha SDL_Init:" + std::string(SDL_GetError()));
        }

        int paramOk = 0;
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)) {
            paramOk++;
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3)) {
                paramOk++;
                if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)) {
                    paramOk++;
                    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)) {
                        paramOk++;
                        if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)) {
                            paramOk++;
                        }
                    }
                }
            }
        }

        if (paramOk != 5) {
            throw std::runtime_error(std::format("Parametro: {}  erro: {}", paramOk, std::string(SDL_GetError())));
        }

        this->window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (this->window == nullptr) {
            throw std::runtime_error("create Window:" + std::string(SDL_GetError()));
        }

        this->context = SDL_GL_CreateContext(window);
        if (this->context == nullptr) {
            throw std::runtime_error("create context:" + std::string(SDL_GetError()));
        }

        // Opcional: Ativar VSync (1) ou desativar (0)
        if (!SDL_GL_SetSwapInterval(1)) {
            throw std::runtime_error("SetSwapInterval:" + std::string(SDL_GetError()));
        }

        if (not SDL_GL_MakeCurrent(this->window, this->context)) {
            throw std::runtime_error("MakeCurrent:" + std::string(SDL_GetError()));
        }

        // iniciala GLEW
        glewExperimental = GL_TRUE;
        if (GLenum err = glewInit(); err != GLEW_OK) {

            if (err == GLEW_ERROR_NO_GLX_DISPLAY) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLEW init fail maybe wayland");
            } else {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLEW init: %s", glewGetErrorString(err));
                throw std::runtime_error("GLEW Init fail");
            }
        }

        // SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowPosition(window, &posX, &posY);

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
        if (this->context != nullptr) {
            SDL_GL_DestroyContext(this->context);
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

        if (GLenum erro = glGetError(); erro != GL_NO_ERROR) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL Erro: %d", erro);
        }

        SDL_GL_SwapWindow(window);
    }

    void CanvasGL::reshape(int _width, int _height) {
        width = _width;
        height = _height;
    }

    void CanvasGL::toggleFullScreen() {

        if (!fullScreen) {
            SDL_GetWindowPosition(window, &posX, &posY);
            SDL_SetWindowPosition(window, 0, 0);
            SDL_SetWindowFullscreen(window, true);
        } else {
            SDL_SetWindowFullscreen(window, false);
            SDL_SetWindowPosition(window, posX, posY);
        }

        fullScreen = !fullScreen;
    }
} // namespace ce
