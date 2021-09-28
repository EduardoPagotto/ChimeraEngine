#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/2d/Group.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include "chimera/render/FontManager.hpp"
#include "chimera/render/Texture.hpp"
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

    shader = new Shader("basic", "./samples/helloworld/basic.vert", "./samples/helloworld/basic.frag");
    shader->enable();
    GLint texIDs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    shader->setUniform1iv("textures", 10, texIDs);
    shader->setUniform2fv("light_pos", 1, glm::value_ptr(glm::vec2(4.0f, 1.5f)));

    layer = new TileLayer(shader);
    // Group* group = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 5.0f, 0.0f)));
    // group->add(new Sprite(0.0f, 0.0f, 6.0f, 3.0f, glm::vec4(1, 1, 1, 1)));
    // Group* button = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
    // button->add(new Sprite(0.0f, 0.0f, 5.0f, 2.0f, glm::vec4(1, 0, 1, 1)));
    // button->add(new Sprite(0.5f, 0.5f, 3.0f, 1.0f, glm::vec4(0.2f, 0.3f, 0.8f, 1)));
    // group->add(button);
    // layer->add(group);

    // parei em :https://www.youtube.com/watch?v=S_99R9DZF6c&t=23s

    Texture* texture[] = {new TextureImg("tex1", "./data/images/grid2.png"), new TextureImg("tex2", "./data/images/grid1.png"),
                          new TextureImg("tex3", "./data/images/grid3.png")};

    texture[0]->init();
    texture[1]->init();
    texture[2]->init();

    // layer = new TileLayer(shader);
    // for (float y = -9.0f; y < 9.0f; y++) {
    //     for (float x = -16.0f; x < 16.0f; x++) {
    //         if (rand() % 4 == 0)
    //             layer->add(new Sprite(x, y, 0.9f, 0.9f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
    //         else
    //             layer->add(new Sprite(x, y, 0.9f, 0.9f, texture[rand() % 3]));
    //     }
    // }

    FontManager::add(new Chimera::FontAtlas("FreeSans_18", "./samples/models/fonts/FreeSans.ttf", 22));
    Label* lFPS = new Label("ABCDEF", -15.0f, 0.0f, "FreeSans_18", glm::vec4(1.0, 1.0, 1.0, 1.0));
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

    layer->render();

    shader->disable();

    pVideo->after();
    pVideo->swapWindow();
}
