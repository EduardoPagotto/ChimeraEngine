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

    // glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color //FIXME: colocar so scene
    glClearColor(0.1F, 0.2F, 0.4F, 1.0F); // Initialize clear color
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0F);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

void Game::onEvent(const SDL_Event& event) {}

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
