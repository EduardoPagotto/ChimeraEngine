#include "Game.hpp"
#include "chimera/base/event.hpp"
#include "chimera/core/gl/OpenGLDefs.hpp"

Game::Game() : IStateMachine("Game") {
    keyboard = ce::g_service_locator.getService<ce::IKeyboard>();
    mouse = ce::g_service_locator.getService<ce::IMouse>();
}

Game::~Game() {
    keyboard = nullptr;
    mouse = nullptr;
}

void Game::onAttach() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color //FIXME: colocar so scene
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

bool Game::onEvent(const SDL_Event& event) {
    using namespace ce;

    mouse->getEvent(event);
    keyboard->getEvent(event);

    switch (event.type) {
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    sendChimeraEvent(EventCE::FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    sendChimeraEvent(EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                default:
                    break;
            }
        } break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    sendChimeraEvent(EventCE::FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    sendChimeraEvent(EventCE::FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate(const double& ts) {

    using namespace ce;

    // if (scene->isReady()) {
    //     NativeScriptComponent& cc = scene->getRegistry().findComponent<NativeScriptComponent>("CameraController");
    //     // cam = (ICamera3D*)cc.camera;

    //     glm::vec3 pos = cc.getPosition();
    //     glm::vec3 front = cc.getFront();
    //     glm::vec3 up = cc.getUp();

    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "pos(%.2f; %.2f; %.2f)", pos.x, pos.y, pos.z);
    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "front(%.2f; %.2f; %.2f)", front.x, front.y, front.z);
    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "up(%.2f; %.2f; %.2f)", up.x, up.y, up.z);
    // }
}

void Game::onRender() {}
