#include "Game.hpp"
#include "chimera_base/event.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"

Game::Game(std::shared_ptr<entt::registry> registry) : registry_(registry) {

    this->canva_ = std::dynamic_pointer_cast<ce::CanvasGL>(registry->ctx().get<std::shared_ptr<ce::ICanva>>());
    if (this->canva_ == nullptr) {
        throw std::runtime_error("Canva not found in CTX");
    }

    this->input_manager_ = registry->ctx().get<std::shared_ptr<ce::InputManager>>();
}

Game::~Game() {}

void Game::on_attach() {

    // glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color //FIXME: colocar so scene
    glClearColor(0.1F, 0.2F, 0.4F, 1.0F); // Initialize clear color
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0F);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::on_deatach() {}

void Game::on_event(const SDL_Event& event) {}

void Game::on_update(const double& ts) {
    using namespace ce;

    if (this->input_manager_->getKeyboard()->isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->input_manager_->getKeyboard()->isKeyPressed(SDL_SCANCODE_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }
}

void Game::on_render() {}
