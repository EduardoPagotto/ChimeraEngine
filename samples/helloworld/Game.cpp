#include "Game.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include "chimera/render/2d/layer/Group.hpp"
#include "chimera/render/FontManager.hpp"
#include <time.h>

Game::Game(Chimera::Canvas* canvas) : Application(canvas) {

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

    TextureManager::loadFromFile("t01", "./assets/textures/grid1.png", TexParam());
    TextureManager::loadFromFile("t02", "./assets/textures/grid2.png", TexParam());
    TextureManager::loadFromFile("t03", "./assets/textures/grid3.png", TexParam());
}

Game::~Game() {}

void Game::onStart() {
    using namespace Chimera; // 26:10 -> https://www.youtube.com/watch?v=wYVaIOUhz6s&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=96 (video
                             // 96)
                             // video 103 finaliza o pick mouse
                             // colocar para rodar o scene como renderbuffer!!!!!!!!!

    ShaderManager::load("./assets/shaders/Basic2D.glsl", shader);

    layer = new TileLayer(&shader);
    for (float y = -9.0f; y < 9.0f; y++) {
        for (float x = -16.0f; x < 16.0f; x++) {
            if (rand() % 4 == 0)
                layer->add(new Sprite(x, y, 0.9f, 0.9f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
            else
                layer->add(new Sprite(x, y, 0.9f, 0.9f, TextureManager::getIndex(rand() % 3)));
        }
    }

    FontManager::add(new Chimera::FontAtlas("FreeSans_22", "./assets/fonts/FreeSans.ttf", 22));
    FontManager::get()->setScale(glm::vec2(canvas->getWidth() / 32.0f, canvas->getHeight() / 18.0f)); // em TileLayer ortho values!!!
    lFPS = new Label("None", -15.5f, 7.8f, glm::vec4(1.0, 1.0, 1.0, 1.0));
    layer->add(lFPS);
    this->pushLayer(layer);
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case EVENT_TOGGLE_FULL_SCREEN:
                    canvas->toggleFullScreen();
                    break;
                case EVENT_NEW_FPS: {
                    uint32_t* pFps = (uint32_t*)event.user.data1;
                    fps = *pFps;
                    SDL_Log("FPS: %d", fps);
                } break;
            }

        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    utilSendEvent(EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    utilSendEvent(EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
            }
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    utilSendEvent(EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    utilSendEvent(EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    canvas->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate() {

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));

    canvas->before();
    layer->render();
    canvas->after();
    canvas->swapWindow();
}
