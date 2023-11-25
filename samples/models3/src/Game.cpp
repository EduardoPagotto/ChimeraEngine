#include "Game.hpp"
#include "chimera/core/device/JoystickManager.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/2d/Group.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Scene& scene) : pCorpoRigido(nullptr), scene(&scene) {
    Chimera::JoystickManager::init();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Constructor Game");
}

Game::~Game() {
    Chimera::JoystickManager::release();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Destructor Game");
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    JoystickManager::getEvent(event);
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
                    // case SDLK_F1:
                    //     pHUD->setOn(!pHUD->isOn());
                    // break;
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

        case SDL_JOYAXISMOTION:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
        case SDL_JOYHATMOTION:
        case SDL_JOYBALLMOTION: {

            // Captura joystick 0 se existir
            if (JoystickState* pJoy = JoystickManager::select(event.jball.which); pJoy != nullptr) {

                pJoy->debug();

                float propulsaoPrincipal = 3.0f;

                crt.yaw = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_X, 32));
                crt.pitch = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_Y, 32));
                crt.roll = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::RIGHT_X, 32));

                double throttle = 0;
                crt.throttle = -propulsaoPrincipal * ((1 + JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_TRIGGER))) / 2);

                if (pJoy->getButtonState((uint8_t)JOY_BUTTON_COD::X) == SDL_PRESSED) {
                    // glm::vec3 posicao = pEmissor->getPosSource();
                    // posicao.x = posicao.x - 0.1f;
                    // pEmissor->setPosSource( posicao );
                }

                if (pJoy->getButtonState((uint8_t)JOY_BUTTON_COD::B) == SDL_PRESSED) { // ( or SDL_RELEASED)
                    // glm::vec3 posicao = pEmissor->getPosSource();
                    // posicao.x = posicao.x + 0.1f;
                    // pEmissor->setPosSource(posicao);
                }

                crt.hat = pJoy->getHat(0);
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

void Game::onAttach() {

    using namespace Chimera;
    // Localiza objeto como o primario
    TransComponent& tc = scene->getRegistry()->findComponent<TransComponent>("Zoltan");
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

    if (pCorpoRigido)
        scene->setOrigem(pCorpoRigido);

    using namespace Chimera;

    float propulsaoLRUD = 5.0f;
    float torque = 0.5f;

    if (pCorpoRigido) {
        if (crt.hat & (uint8_t)SDL_HAT_UP)
            pCorpoRigido->applyForce(glm::vec3(0.0, 0.0, propulsaoLRUD));

        if (crt.hat & (uint8_t)SDL_HAT_DOWN)
            pCorpoRigido->applyForce(glm::vec3(0.0, 0.0, -propulsaoLRUD));

        if (crt.hat & (uint8_t)SDL_HAT_RIGHTUP)
            pCorpoRigido->applyForce(glm::vec3(propulsaoLRUD, 0.0, 0.0));

        if (crt.hat & (uint8_t)SDL_HAT_LEFTUP)
            pCorpoRigido->applyForce(glm::vec3(-propulsaoLRUD, 0.0, 0.0));

        if ((crt.roll != 0.0) || (crt.pitch != 0.0) || (crt.yaw != 0.0) || (crt.throttle != 0.0)) {
            pCorpoRigido->applyForce(glm::vec3(0.0, crt.throttle, 0.0));
            pCorpoRigido->applyTorc(glm::vec3(-torque * crt.pitch, -torque * crt.roll, -torque * crt.yaw));
        }
    }

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));
}

void Game::onRender() {}