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

void Game::mouseEvent(Chimera::Core::MouseDevice* pMouse, SDL_Event* pEventSDL) {

    x = pEventSDL->motion.x;
    y = pEventSDL->motion.y;
}

void Game::start() {
    using namespace Chimera;
    // sprite1 = new StaticSprite(5, 5, 4, 4, glm::vec4(1, 0, 1, 1), shader);
    // sprite2 = new StaticSprite(7, 1, 2, 3, glm::vec4(0.2f, 0, 1, 1), shader);
    sprite1 = new Sprite(5, 5, 4, 4, glm::vec4(1, 0, 1, 1));
    sprite2 = new Sprite(7, 1, 2, 3, glm::vec4(0.2f, 0, 1, 1));
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
    shader->setUniform2fv("light_pos", 1, glm::value_ptr(glm::vec2((float)(x * 16.0f / 960.0f), (float)(9.0f - y * 9.0f / 540.0f))));

    render2D.begin();
    render2D.submit(sprite1);
    render2D.submit(sprite2);
    render2D.end();
    render2D.flush();

    shader->disable();

    pVideo->after();
    pVideo->swapWindow();
}
