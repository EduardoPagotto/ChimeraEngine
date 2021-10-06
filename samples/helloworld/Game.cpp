#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include "chimera/render/2d/layer/Group.hpp"
#include "chimera/render/FontManager.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

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

    // tranparence of fonts
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Group* group = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 5.0f, 0.0f)));
    // group->add(new Sprite(0.0f, 0.0f, 6.0f, 3.0f, glm::vec4(1, 1, 1, 1)));
    // Group* button = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
    // button->add(new Sprite(0.0f, 0.0f, 5.0f, 2.0f, glm::vec4(1, 0, 1, 1)));
    // button->add(new Sprite(0.5f, 0.5f, 3.0f, 1.0f, glm::vec4(0.2f, 0.3f, 0.8f, 1)));
    // group->add(button);
    // layer->add(group);

    TextureManager::loadFromFile("t01", "./data/images/grid1.png", TextureParameters());
    TextureManager::loadFromFile("t02", "./data/images/grid2.png", TextureParameters());
    TextureManager::loadFromFile("t03", "./data/images/grid3.png", TextureParameters());

    sl.load("./assets/shaders/Basic2D.glsl");
    shader = sl.get("Basic2D");
    shader->enable();

    layer = new TileLayer(shader);
    for (float y = -9.0f; y < 9.0f; y++) {
        for (float x = -16.0f; x < 16.0f; x++) {
            if (rand() % 4 == 0)
                layer->add(new Sprite(x, y, 0.9f, 0.9f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
            else
                layer->add(new Sprite(x, y, 0.9f, 0.9f, TextureManager::getIndex(rand() % 3)));
        }
    }

    FontManager::add(new Chimera::FontAtlas("FreeSans_22", "./samples/models/fonts/FreeSans.ttf", 22));
    FontManager::get()->setScale(glm::vec2(pVideo->getWidth() / 32.0f, pVideo->getHeight() / 18.0f)); // em TileLayer ortho values!!!
    lFPS = new Label("None", -15.5f, 7.8f, glm::vec4(1.0, 1.0, 1.0, 1.0));
    layer->add(lFPS);

    shader->disable();
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
            uint32_t* pFps = (uint32_t*)_event.user.data1;
            fps = *pFps;
            SDL_Log("FPS: %d", fps);
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

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));

    pVideo->before();
    shader->enable();
    shader->setUniform2fv("light_pos", 1,
                          glm::value_ptr(glm::vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f))));

    layer->render();

    shader->disable();

    pVideo->after();
    pVideo->swapWindow();
}
