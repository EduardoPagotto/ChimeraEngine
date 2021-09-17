#include "Game.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/core/io/utils.hpp"
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::Core::CanvasGL* _pVideo) : pVideo(_pVideo) {

    shader = new Chimera::Shader("basic", "./samples/helloworld/basic.vert", "./samples/helloworld/basic.frag");
}

Game::~Game() {}

void Game::joystickEvent(Chimera::Core::JoystickState* pJoy, SDL_Event* pEventSDL) {}

void Game::keboardEvent(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_STOP, nullptr, nullptr);
            break;
        case SDLK_F10:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseEvent(Chimera::Core::MouseDevice* pMouse, SDL_Event* pEventSDL) {}

void Game::start() {
    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    Chimera::VertexData v[] = {{glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0)},
                               {glm::vec3(2.0f, 4.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0)},
                               {glm::vec3(4.0f, 4.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0)},
                               {glm::vec3(4.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0)}};
    const uint32_t sv = 4;

    uint32_t i[] = {0, 1, 2, 2, 3, 0};
    const uint32_t si = 6;

    renderable = new Chimera::RenderableStatic(v, sv, i, si);
}

void Game::userEvent(const SDL_Event& _event) {
    switch (_event.user.code) {
        case Chimera::Core::EVENT_TOGGLE_FULL_SCREEN:
            pVideo->toggleFullScreen();
            break;
        case Chimera::Core::EVENT_FLOW_START:
            this->start();
        default:
            break;
    }
}

void Game::windowEvent(const SDL_WindowEvent& _event) {
    switch (_event.event) {
        case SDL_WINDOWEVENT_ENTER:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_RESUME, nullptr, nullptr);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_PAUSE, nullptr, nullptr);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pVideo->reshape(_event.data1, _event.data2);
            break;
        default:
            break;
    }
}

void Game::update() {
    pVideo->before();

    shader->enable();

    glm::mat4 orto = glm::ortho(0.0f, 16.f, 0.0f, 9.0f, -1.0f, 1.0f);
    shader->setUniformMatrix4fv("pr_matrix", 1, false, glm::value_ptr(orto));
    shader->setUniform4fv("colour", 1, glm::value_ptr(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f)));
    shader->setUniform2fv("light_pos", 1, glm::value_ptr(glm::vec2(3.0f, 3.0f)));

    glm::mat4 modelMatrix(1.0f);
    glm::mat4 mm = glm::translate(modelMatrix, glm::vec3(2.0f, 2.0f, 0.0f));
    // glm::mat4 mm = glm::rotate(modelMatrix, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    shader->setUniformMatrix4fv("ml_matrix", 1, false, glm::value_ptr(mm));

    reder3d.submit(renderable);
    reder3d.flush();

    shader->disable();

    pVideo->after();
    pVideo->swapWindow();
}
