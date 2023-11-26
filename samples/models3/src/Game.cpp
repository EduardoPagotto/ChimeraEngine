#include "Game.hpp"
#include "chimera/core/device/GameControllerManager.hpp"
#include "chimera/core/device/JoystickManager.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/2d/Group.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Scene& scene) : IStateMachine("Game"), pCorpoRigido(nullptr), scene(&scene) {
    Chimera::JoystickManager::init();
    // Chimera::GameControllerManager::init();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Constructor Game");
    registry = Chimera::RegistryManager::getPtr();
}

Game::~Game() {
    Chimera::JoystickManager::release();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Destructor Game");
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    JoystickManager::getEvent(event);
    // GameControllerManager::getEvent(event);
    MouseDevice::getEvent(event);

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case Chimera::EVENT_COLLIDE_START: {
                    uint32_t* n1 = (uint32_t*)event.user.data1;
                    uint32_t* n2 = (uint32_t*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao start: %d -> %d", *n1, *n2);
                } break;
                case Chimera::EVENT_COLLIDE_ON: {
                    uint32_t* n1 = (uint32_t*)event.user.data1;
                    uint32_t* n2 = (uint32_t*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao ON: %d -> %d", *n1, *n2);
                } break;
                case Chimera::EVENT_COLLIDE_OFF: {
                    uint32_t* n1 = (uint32_t*)event.user.data1;
                    uint32_t* n2 = (uint32_t*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao OFF: %d -> %d", *n1, *n2);
                } break;
                case Chimera::EVENT_NEW_FPS: {
                    uint32_t* pFps = (uint32_t*)event.user.data1;
                    fps = *pFps;
                    // glm::vec3 val1 = pCorpoRigido->getPosition();
                    // sPosicaoObj = "pos:(" + std::to_string(val1.x) + "," + std::to_string(val1.y) + "," + std::to_string(val1.z) + ")";
                } break;
                default:
                    break;
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
                case SDLK_UP:
                    pCorpoRigido->applyForce(glm::vec3(10.0, 0.0, 0.0));
                    break;
                case SDLK_DOWN:
                    pCorpoRigido->applyForce(glm::vec3(-10.0, 0.0, 0.0));
                    break;
                case SDLK_LEFT:
                    pCorpoRigido->applyForce(glm::vec3(0.0, 10.0, 0.0));
                    break;
                case SDLK_RIGHT:
                    pCorpoRigido->applyForce(glm::vec3(0.0, -10.0, 0.0));
                    break;
                case SDLK_a:
                    pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, 10.0));
                    break;
                case SDLK_s:
                    pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, -10.0));
                    break;
                default:
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

void Game::onAttach() {

    using namespace Chimera;
    // Localiza objeto como o primario
    TransComponent& tc = registry->findComponent<TransComponent>("Zoltan");
    pCorpoRigido = (Solid*)tc.trans;

    lFPS = new Label("None", 0, 0, glm::vec4(1.0, 1.0, 1.0, 1.0));
    Tile* tile = (Tile*)scene->getLayes().getState("TileText");

    Group* group = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, 270.0f, 0.0f)));
    group->add(new Sprite(0.0f, 0.0f, 200.0f, 50.0f, glm::vec4(0, 0, 1, 0.25)));
    group->add(lFPS);
    tile->add(group);
    // tile->add(lFPS);

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

void Game::onUpdate(Chimera::ViewProjection& vp, const double& ts) {

    using namespace Chimera;

    if (pCorpoRigido)
        scene->setOrigem(pCorpoRigido);

    if (SDL_Joystick* pJoy = JoystickManager::get(0); pJoy != nullptr) {

        float propulsaoLRUD = 5.0f;
        uint ggg = SDL_JoystickGetHat(pJoy, 0);
        switch (ggg) {
            case SDL_HAT_UP:
                pCorpoRigido->applyForce(glm::vec3(0.0, 0.0, propulsaoLRUD));
                break;
            case SDL_HAT_DOWN:
                pCorpoRigido->applyForce(glm::vec3(0.0, 0.0, -propulsaoLRUD));
                break;
            case SDL_HAT_LEFT:
                pCorpoRigido->applyForce(glm::vec3(propulsaoLRUD, 0.0, 0.0));
                break;
            case SDL_HAT_RIGHT:
                pCorpoRigido->applyForce(glm::vec3(-propulsaoLRUD, 0.0, 0.0));
                break;
            default:
                break;
        }

        // JoystickManager::debugJoy(*pJoy);

        int16_t deadZone = 128;

        float yaw = scale16(dead16(SDL_JoystickGetAxis(pJoy, 0), deadZone), 0x8000);   // LEFTX
        float pitch = scale16(dead16(SDL_JoystickGetAxis(pJoy, 1), deadZone), 0x8000); // LEFTY
        float roll = scale16(dead16(SDL_JoystickGetAxis(pJoy, 2), deadZone), 0x8000);  // RIFHT_X
        float acc = scale16(dead16(SDL_JoystickGetAxis(pJoy, 3), deadZone), 0x8000);   // LEFT_TRIGGER
        float throttle = -3.0f * (acc / 2);

        if (SDL_JoystickGetButton(pJoy, 0) == SDL_PRESSED) { // JOY_BUTTON_COD::X
            // glm::vec3 posicao = pEmissor->getPosSource();
            // posicao.x = posicao.x - 0.1f;
            // pEmissor->setPosSource( posicao );
        }

        if (SDL_JoystickGetButton(pJoy, 1) == SDL_PRESSED) { // ( or SDL_RELEASED)
            // glm::vec3 posicao = pEmissor->getPosSource();
            // posicao.x = posicao.x + 0.1f;
            // pEmissor->setPosSource(posicao);
        }

        if ((roll != 0.0) || (pitch != 0.0) || (yaw != 0.0) || (throttle != 0.0)) {
            float torque = 0.5f;
            pCorpoRigido->applyForce(glm::vec3(0.0, throttle, 0.0));
            pCorpoRigido->applyTorc(glm::vec3(-torque * pitch, -torque * roll, -torque * yaw));
        }
    }

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));
}

void Game::onRender() {}