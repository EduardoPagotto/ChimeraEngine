#include "Game.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/core/io/utils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define batch 1

Game::Game(Chimera::Core::CanvasGL* _pVideo) : pVideo(_pVideo) {}

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

    srand(time(nullptr));

    shader = new Shader("basic", "./samples/helloworld/basic.vert", "./samples/helloworld/basic.frag");
    shader2 = new Shader("basic", "./samples/helloworld/basic.vert", "./samples/helloworld/basic.frag");

    shader->enable();
    shader2->enable();

    shader->setUniform2fv("light_pos", 1, glm::value_ptr(glm::vec2(4.0f, 1.5f)));
    shader2->setUniform2fv("light_pos", 1, glm::value_ptr(glm::vec2(4.0f, 1.5f)));

    layer = new TileLayer(shader);
    for (float y = -9.0f; y < 9.0f; y++) {
        for (float x = -16.0f; x < 16.0f; x++) {
            layer->add(new Sprite(x, y, 0.9f, 0.9f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
        }
    }

    layer2 = new TileLayer(shader2);
    layer2->add(new Sprite(-2, -2, 4, 4, glm::vec4(1, 0, 1, 1)));
}

void Game::userEvent(const SDL_Event& _event) {
    switch (_event.user.code) {
        case Chimera::Core::EVENT_TOGGLE_FULL_SCREEN:
            pVideo->toggleFullScreen();
            break;
        case Chimera::Core::EVENT_FLOW_START:
            this->start();
            break;
        case Chimera::Core::EVENT_NEW_FPS: {
            uint32_t* fps = (uint32_t*)_event.user.data1;
            SDL_Log("FPS: %d", *fps);
        } break;
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
    shader->setUniform2fv("light_pos", 1,
                          glm::value_ptr(glm::vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f))));

    shader2->enable();
    shader2->setUniform2fv("light_pos", 1,
                           glm::value_ptr(glm::vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f))));

    layer->render();
    layer2->render();

    shader->disable();

    pVideo->after();
    pVideo->swapWindow();
}
