#include "Game.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/utils.hpp"

Game::Game(Chimera::Scene& scene) : IStateMachine("Game"), scene(&scene) {}

Game::~Game() {}

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
    using namespace Chimera;

    MouseDevice::getEvent(event);
    Keyboard::getEvent(event);

    switch (event.type) {
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    utilSendEvent(EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    utilSendEvent(EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
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

void Game::onUpdate(Chimera::ViewProjection& vp, const double& ts) {

    using namespace Chimera;

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