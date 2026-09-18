#include "Game.hpp"
#include "chimera_base/event.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"

Game::Game(std::shared_ptr<entt::registry> registry) : registry(registry) {

    this->canva = std::dynamic_pointer_cast<ce::CanvasGL>(registry->ctx().get<std::shared_ptr<ce::ICanva>>());
    if (this->canva == nullptr) {
        throw std::runtime_error("Canva not found in CTX");
    }

    this->inputManager = registry->ctx().get<std::shared_ptr<ce::InputManager>>();
}

Game::~Game() {}

void Game::onAttach() {

    // glClearColor(0.F, 0.F, 0.F, 1.F); // Initialize clear color
    glClearColor(0.1F, 0.2F, 0.4F, 1.0F); // Initialize clear color

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0F);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

void Game::onEvent(const SDL_Event& event) {
    // using namespace ce;

    // mouse->getEvent(event);

    // switch (event.type) {
    //     case SDL_EVENT_KEY_DOWN: {
    //         switch (event.key.key) {
    //             case SDLK_ESCAPE:
    //                 sendChimeraEvent(EventCE::FLOW_STOP, nullptr, nullptr);
    //                 break;
    //             case SDLK_F10:
    //                 sendChimeraEvent(EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
    //                 break;
    //         }
    //     } break;
    //     case SDL_EVENT_MOUSE_BUTTON_DOWN:
    //     case SDL_EVENT_MOUSE_BUTTON_UP:
    //     case SDL_EVENT_MOUSE_MOTION: {
    //     } break;
    //     case SDL_EVENT_WINDOW_MOUSE_ENTER:
    //         sendChimeraEvent(EventCE::FLOW_RESUME, nullptr, nullptr); // isPaused = false;
    //         break;
    //     case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    //         sendChimeraEvent(EventCE::FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
    //         break;
    // }
}

void Game::onUpdate(const double& ts) {
    using namespace ce;

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }
}

void Game::onRender() {}
