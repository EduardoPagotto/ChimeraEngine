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

void Game::onStart() {
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
    // FIXME: ver como sera???
    // layerStack->pushLayer(layer);
    layer->onAttach();

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

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;
    using namespace Chimera::Core;
    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case EVENT_TOGGLE_FULL_SCREEN:
                    pVideo->toggleFullScreen();
                    break;

                case Chimera::Core::EVENT_FLOW_START: {
                    layerStack = (Chimera::Core::LayerStack*)event.user.data1;
                    this->onStart();
                } break;

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
                // case SDLK_1:
                //     render3d.logToggle();
                //     break;
                case SDLK_F10:
                    utilSendEvent(EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
            }
        } break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
            x = event.motion.x;
            y = event.motion.y;
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
                    pVideo->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate() {

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
