#include "Game.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/core/io/utils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define batch 1

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

    srand(time(nullptr));

    for (float y = 0; y < 9.0f; y++) {
        for (float x = 0; x < 16.0f; x++) {
#if batch
            sprites.push_back(new Sprite(x, y, 0.9f, 0.9f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
#else
            sprites.push_back(new StaticSprite(x, y, 0.9f, 0.9f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1), shader));
#endif
        }
    }

    //     for (float y = 0; y < 9.0f; y += 0.1) {
    //         for (float x = 0; x < 16.0f; x += 0.1) {
    // #if batch
    //             sprites.push_back(new Sprite(x, y, 0.09f, 0.09f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
    // #else
    //             sprites.push_back(new StaticSprite(x, y, 0.09f, 0.09f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1), shader));
    // #endif
    //         }
    //     }
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

#if batch
    batchRender2D.begin();
    for (auto sprite : sprites)
        batchRender2D.submit(sprite);
    batchRender2D.end();
    batchRender2D.flush();
#else
    for (auto sprite : sprites)
        simpleRender2D.submit(sprite);
    simpleRender2D.flush();
#endif

    shader->disable();

    pVideo->after();
    pVideo->swapWindow();
}
