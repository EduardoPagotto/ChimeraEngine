#include "Game.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/core/visible/FontManager.hpp"
#include "chimera/core/visible/TextureManager.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/2d/Group.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include <time.h>

Game::Game(std::shared_ptr<ServiceLocator> sl, Chimera::Engine* engine) : IStateMachine("Game"), engine(engine) {

    using namespace Chimera;
    srand(time(nullptr));
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

    canvas = sl->getService<ICanva>();
}

Game::~Game() {}

void Game::onAttach() {

    // ApplicationGL::onAttach();

    using namespace Chimera; // 26:10 -> https://www.youtube.com/watch?v=wYVaIOUhz6s&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=96 (video
                             // 96)
                             // video 103 finaliza o pick mouse
                             // colocar para rodar o scene como renderbuffer!!!!!!!!!

    std::unordered_map<GLenum, std::string> shadeData;
    shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Basic2D.frag";
    shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Basic2D.vert";
    ShaderManager::load("Basic2D", shadeData, shader);

    layer = new TileLayer(shader);
    layer->getCamera()->setViewportSize(canvas->getWidth(), canvas->getHeight());

    for (float y = -8.0f; y < 8.0f; y++) {
        for (float x = -14.0f; x < 14.0f; x++) {
            if (rand() % 4 == 0)
                layer->add(new Sprite(x, y, 1.0f, 1.0f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
            else
                layer->add(new Sprite(x, y, 1.0f, 1.0f, TextureManager::getIndex(rand() % 3)));
        }
    }

    FontManager::add(new Chimera::FontAtlas("FreeSans_22", "./assets/fonts/FreeSans.ttf", 22));
    FontManager::get()->setScale(glm::vec2(0.04, 0.04)); // em TileLayer ortho values!!!
    lFPS = new Label("None", 0, 0, glm::vec4(1.0, 1.0, 1.0, 1.0));
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
    using namespace Chimera;

    // if (ApplicationGL::onEvent(event) == false)
    //     return false;

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
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
            }
        } break;
    }
    return true;
}

void Game::onUpdate(Chimera::IViewProjection& vp, const double& ts) {

    // ApplicationGL::onUpdate();

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));
}
