#include "chimera_core/gl/CanvasGL.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"
#include <SDL3/SDL_log.h>
#include <format>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

namespace ce {

    inline void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                    const GLchar* message, const void* userParam) {

        std::string sev;
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                sev = "CRITICAL:";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                sev = "WARN:";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                sev = "INFO:";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                sev = "NOTIFICATION:";
                break;
            default:
                sev = "UNKNOWN:";
                break;
        }

        SDL_Log("[OpenGL Debug] %s (ID: %d)", sev.c_str(), id);

        // Se for um erro crítico, você pode forçar um breakpoint no debugger
        if (severity == GL_DEBUG_SEVERITY_HIGH) {
            __builtin_trap(); // Para Linux/GCC/Clang
            // __debugbreak();   // Para Windows/MSVC
        }
    }

    CanvasGL::CanvasGL(const std::string& title, int width, int height, bool fullScreen) // NOLINT
        : title(title), width(width), height(height), fullScreen(fullScreen), window(nullptr) {

        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland")) {
            throw std::runtime_error(std::format("SDL wayland Failed driver: {}", SDL_GetError()));
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Falha SDL_Init:" + std::string(SDL_GetError()));
        }

        int paramOk = 0;
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)) {
            paramOk++;
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6)) {
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

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        this->window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (this->window == nullptr) {
            throw std::runtime_error("create Window:" + std::string(SDL_GetError()));
        }

        this->context = SDL_GL_CreateContext(window);
        if (this->context == nullptr) {
            throw std::runtime_error("create context:" + std::string(SDL_GetError()));
        }

        // 0 = Desativado (máximo de FPS), 1 = Sincronizado, -1 = Adaptive VSync
        if (!SDL_GL_SetSwapInterval(1)) {
            throw std::runtime_error("SetSwapInterval:" + std::string(SDL_GetError()));
        }

        CarregarOpenGL();

        if (glGetIntegerv != nullptr) {
            GLint flags = 0;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0) {
                SDL_Log("[Sucesso] O Driver confirmou um contexto com suporte a DEBUG nativo.");
            } else {
                SDL_Log("[Aviso] O Driver IGNOROU o pedido de contexto de Debug.");
            }
        }

        if (not SDL_GL_MakeCurrent(this->window, this->context)) {
            throw std::runtime_error("MakeCurrent:" + std::string(SDL_GetError()));
        }

        // SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowPosition(window, &posX, &posY);

        SDL_Log("Renderer: %s", glGetString(GL_RENDERER));
        SDL_Log("OpenGL Version: %s", glGetString(GL_VERSION));

        if (glDebugMessageCallback_ptr != nullptr) {
            glEnable(GL_DEBUG_OUTPUT);
            // Garante que o callback rode na mesma Thread permitindo breakpoints fáceis
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback_ptr(OpenGLDebugCallback, nullptr);
            SDL_Log("OpenGL Debug Callback configurado com sucesso!");
        } else {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Não foi possível configurar o Debug Callback (Função indisponível).");
        }
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

    void CanvasGL::after() { SDL_GL_SwapWindow(window); }

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
