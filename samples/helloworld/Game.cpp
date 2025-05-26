#include "Game.hpp"
#include "chimera/base/Transform.hpp"
#include "chimera/base/event.hpp"
#include "chimera/core/gl/FontMng.hpp"
#include "chimera/core/gl/ShaderMng.hpp"
#include "chimera/core/gl/TextureMng.hpp"
#include "chimera/render/2d/Group.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include <time.h>

Game::Game(ce::Engine* engine) : IStateMachine("Game"), engine(engine) {

    using namespace ce;
    srand(time(nullptr));
    // Group* group = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 5.0f, 0.0f)));
    // group->add(new Sprite(0.0f, 0.0f, 6.0f, 3.0f, glm::vec4(1, 1, 1, 1)));
    // Group* button = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
    // button->add(new Sprite(0.0f, 0.0f, 5.0f, 2.0f, glm::vec4(1, 0, 1, 1)));
    // button->add(new Sprite(0.5f, 0.5f, 3.0f, 1.0f, glm::vec4(0.2f, 0.3f, 0.8f, 1)));
    // group->add(button);
    // layer->add(group);
    auto texMng = g_service_locator.getService<TextureMng>();
    texMng->loadFromFile("t01", "./assets/textures/grid1.png", TexParam());
    texMng->loadFromFile("t02", "./assets/textures/grid2.png", TexParam());
    texMng->loadFromFile("t03", "./assets/textures/grid3.png", TexParam());

    canvas = g_service_locator.getService<ICanva>();

    std::unordered_map<GLenum, std::string> shadeData;
    shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Basic2D.frag";
    shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Basic2D.vert";

    auto mng = g_service_locator.getService<ShaderMng>();
    shader = mng->load("Basic2D", shadeData);
}

Game::~Game() {}

void Game::onAttach() {

    // ApplicationGL::onAttach();

    using namespace ce; // 26:10 ->
                        // https://www.youtube.com/watch?v=wYVaIOUhz6s&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=96
                        // (video 96) video 103 finaliza o pick mouse colocar para rodar o scene como
                        // renderbuffer!!!!!!!!!

    layer = new TileLayer(shader);
    layer->getCamera()->setViewportSize(canvas->getWidth(), canvas->getHeight());
    auto texMng = g_service_locator.getService<TextureMng>();

    for (float y = -8.0f; y < 8.0f; y++) {
        for (float x = -14.0f; x < 14.0f; x++) {
            if (rand() % 4 == 0)
                layer->add(new Sprite(x, y, 1.0f, 1.0f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
            else
                layer->add(new Sprite(x, y, 1.0f, 1.0f, texMng->getIndex(rand() % 3)));
        }
    }

    auto fontMng = g_service_locator.getService<FontMng>();
    auto font = fontMng->load("FreeSans_22", "./assets/fonts/FreeSans.ttf", 22);
    font->setScale(glm::vec2(0.04, 0.04));
    lFPS = new Label("None", 0, 0, font, glm::vec4(1.0, 1.0, 1.0, 1.0));
    layer->add(lFPS);
    engine->getStack().pushState(layer);
}

void Game::onDeatach() {
    // ApplicationGL::onDeatach();
}

void Game::onRender() {
    // this->onRender(); // FIXME: ???????
    // ApplicationGL::onRender();
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace ce;

    // if (ApplicationGL::onEvent(event) == false)
    //     return false;

    switch (event.type) {
        case SDL_EVENT_USER: {
#pragma clang diagnostic ignored "-Wswitch"
            switch (static_cast<EventCE>(event.user.code)) {
                case EventCE::NEW_FPS: {
                    uint32_t* pFps = (uint32_t*)event.user.data1;
                    fps = *pFps;
                    SDL_Log("FPS: %d", fps);
                } break;
            }

        } break;
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_ESCAPE:
                    sendChimeraEvent(EventCE::FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    sendChimeraEvent(EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
            }
        } break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            sendChimeraEvent(EventCE::FLOW_RESUME, nullptr, nullptr); // isPaused = false;
            break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            sendChimeraEvent(EventCE::FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
            break;

            // case SDL_WINDOWEVENT: {
            //     switch (event.window.event) {
            //         case SDL_EVENT_WINDOW_MOUSE_ENTER:
            //             sendChimeraEvent(EventCE::FLOW_RESUME, nullptr, nullptr); // isPaused = false;
            //             break;
            //         case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            //             sendChimeraEvent(EventCE::FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
            //             break;
            //     }
            // } break;
    }
    return true;
}

void Game::onUpdate(const double& ts) {

    // ApplicationGL::onUpdate();

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));
}
