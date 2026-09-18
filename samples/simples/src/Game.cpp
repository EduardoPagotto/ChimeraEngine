#include "Game.hpp"
#include "chimera_base/event.hpp"
#include "chimera_core/gl/OpenGLDefs.hpp"
#include <SDL3/SDL_log.h>

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

    // Habilita o depth buffer/culling face
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0F);
    glDepthFunc(GL_LEQUAL);
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

void Game::onEvent(const SDL_Event& event) {

    // switch (event.type) {
    //     case SDL_EVENT_USER: {
    //         switch (static_cast<ce::EventCE>(event.user.code)) {
    //             case ce::EventCE::NEW_FPS: {
    //                 uint32_t* pFps = (uint32_t*)event.user.data1;
    //                 SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "%.3u", *pFps);
    //                 // fps = *pFps;
    //                 //  glm::vec3 val1 = pCorpoRigido->getPosition();
    //                 //  sPosicaoObj = "pos:(" + std::to_string(val1.x) + "," + std::to_string(val1.y) + "," +
    //                 //  std::to_string(val1.z) + ")";
    //             } break;
    //             default:
    //                 break;
    //         }
    //     } break;
    //     default:
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
