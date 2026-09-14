#include "Game.hpp"
#include "chimera_base/event.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"

Game::Game(std::shared_ptr<entt::registry> registry) : registry(registry) {
    this->inputManager = registry->ctx().get<std::shared_ptr<ce::InputManager>>();
}

Game::~Game() {}

void Game::onAttach() {

    // glClearColor(0.F, 0.F, 0.F, 1.F); // Initialize clear color
    glClearColor(0.1F, 0.2F, 0.4F, 1.0F); // Initialize clear color

    // Habilita o depth buffer/culling face
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0F);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

bool Game::onEvent(const SDL_Event& event) {
    // using namespace ce;

    // keyboard->getEvent(event);

    // switch (event.type) {
    //     case SDL_EVENT_WINDOW_MOUSE_ENTER:
    //         ce::sendChimeraEvent(ce::EventCE::FLOW_RESUME, nullptr, nullptr); // isPaused = false;
    //         break;
    //     case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    //         ce::sendChimeraEvent(ce::EventCE::FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
    //         break;
    // }
    return false;
}

void Game::onUpdate(const double& ts) {
    using namespace ce;

    if (this->inputManager->keyboard->isPressed(SDLK_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->inputManager->keyboard->isPressed(SDLK_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }
}

void Game::onRender() {}
