#include "Game.hpp"
#include "chimera/core/Events.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/node/Transform.hpp"

Game::Game(Chimera::SceneMng* _pScenMng, Chimera::CanvasGL* _pVideo, Chimera::PhysicsControl* _physicWorld)
    : pSceneMng(_pScenMng), pVideo(_pVideo) {

    isPaused = false;
    pCorpoRigido = nullptr;
    pEmissor = nullptr;
    pOrbitalCam = nullptr;

    textoFPS = "fps: 0";
    sPosicaoObj = "pos:(,,)";

    log = spdlog::get("chimera");
    log->debug("Constructor Game");

    physicWorld = _physicWorld;
}

Game::~Game() { log->debug("Destructor Game"); }

void Game::joystickCapture(Chimera::JoystickManager& joy) {}

void Game::joystickStatus(Chimera::JoystickManager& joy) {

    using namespace Chimera;
    // Captura joystick 0 se existir
    JoystickState* joystick = joy.getJoystickState(0);
    if (joystick != nullptr) {

        float propulsaoLRUD = 1.0f;
        float propulsaoPrincipal = 3.0f;
        float propulsaoFrontal = 1.0f;
        float torque = 0.1f;
        float deadZone = 0.5f;

        float yaw = joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_X, deadZone);
        float pitch = joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_Y, deadZone);
        float roll = joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_X, deadZone);

        double throttle = -propulsaoPrincipal * ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_TRIGGER, deadZone)) / 2);
        throttle =
            throttle - (-propulsaoFrontal * ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_TRIGGER, deadZone)) / 2));

        if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::X) == true) {

            // glm::vec3 posicao = pEmissor->getPosSource();
            // posicao.x = posicao.x - 0.1f;
            // pEmissor->setPosSource( posicao );
        }

        if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::B) == true) {

            // glm::vec3 posicao = pEmissor->getPosSource();
            // posicao.x = posicao.x + 0.1f;
            // pEmissor->setPosSource(posicao);
        }

        int val = joystick->Hat(0);
        if (val & (uint8_t)JOY_PAD_COD::UP)
            pCorpoRigido->applyForce(glm::vec3(0.0, 0.0, propulsaoLRUD));

        if (val & (uint8_t)JOY_PAD_COD::DOWN)
            pCorpoRigido->applyForce(glm::vec3(0.0, 0.0, -propulsaoLRUD));

        if (val & (uint8_t)JOY_PAD_COD::RIGHT)
            pCorpoRigido->applyForce(glm::vec3(propulsaoLRUD, 0.0, 0.0));

        if (val & (uint8_t)JOY_PAD_COD::LEFT)
            pCorpoRigido->applyForce(glm::vec3(-propulsaoLRUD, 0.0, 0.0));

        if ((roll != 0.0) || (pitch != 0.0) || (yaw != 0.0) || (throttle != 0.0)) {
            pCorpoRigido->applyForce(glm::vec3(0.0, throttle, 0.0));
            pCorpoRigido->applyTorc(glm::vec3(-torque * pitch, -torque * roll, -torque * yaw));
        }
    }
}

void Game::keyCapture(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::Exception(std::string(SDL_GetError()));
            }
            break;
        case SDLK_F1:
            pHUD->setOn(!pHUD->isOn());
            break;
        case SDLK_F10:
            Chimera::eventsSend(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
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
}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {
    botaoIndex = mb.button;
    estadoBotao = mb.state;
}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {
    botaoIndex = mb.button;
    estadoBotao = mb.state;
}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {

    if (estadoBotao == SDL_PRESSED) {
        if (botaoIndex == 1) {
            pOrbitalCam->getTrackBall()->tracking(mm.yrel, mm.xrel, 0);
        } else if (botaoIndex == 3) {
            pOrbitalCam->getTrackBall()->tracking(0, 0, mm.yrel);
        }
    }
}

void Game::start() {

    pSceneMng->start(pVideo);

    // Localiza o Skybox e ajusta iluminacao
    Chimera::Transform* pSkyBox = (Chimera::Transform*)pSceneMng->findChild("SkyBox", true);

    if (pSkyBox != nullptr) {
        Chimera::Draw* pDraw = (Chimera::Draw*)pSkyBox->findChild(Chimera::EntityKind::MESH, 0, false);
        pSkyBox->setStaticTranslation(true);
    }

    // Localiza a camera
    pOrbitalCam = (Chimera::Camera*)pSceneMng->findChild("Camera-camera", true);

    // Localiza objeto como o primario
    pCorpoRigido = (Chimera::Solid*)pSceneMng->findChild("zoltan-RigidBody", true);

    // Localiza a luz ativa
    Chimera::Light* pLight = (Chimera::Light*)pSceneMng->findChild("luz01-light", true);

    // Localiza o Emissor de particula
    pEmissor = (Chimera::ParticleEmitter*)pSceneMng->findChild("testeZ1", true);

    pSceneMng->origemDesenho((Chimera::Coord*)pCorpoRigido);

    // Localiza o HUD
    pHUD = (Chimera::HUD*)pSceneMng->findChild("HUD-Default", true);
    if (pHUD != nullptr) {
        pHUD->addText(0, 350, 30, Chimera::Color::BLUE, 1.0, &sPosicaoObj);
        pHUD->addText(0, 10, 30, Chimera::Color::RED, 1.0, &textoFPS);
    }
}

void Game::stop() {}

void Game::newFPS(const unsigned int& fps) {

    glm::vec3 val1 = pCorpoRigido->getPosition();
    sPosicaoObj = "pos:(" + std::to_string(val1.x) + "," + std::to_string(val1.y) + "," + std::to_string(val1.z) + ")";
    textoFPS = "fps: " + std::to_string(fps) + std::string(" Periodo: ") + std::to_string(physicWorld->getLastPeriod());
}

void Game::render() {

    physicWorld->stepSim();
    physicWorld->checkCollisions();

    pSceneMng->draw(pVideo);
}

void Game::userEvent(const SDL_Event& _event) {

    Chimera::KindOp op = (Chimera::KindOp)_event.user.code;
    Chimera::Node* n1 = (Chimera::Node*)_event.user.data1;
    Chimera::Node* n2 = (Chimera::Node*)_event.user.data2;

    switch (op) {
        case Chimera::KindOp::START_COLLIDE: {
            Chimera::Node* n1 = (Chimera::Node*)_event.user.data1;
            Chimera::Node* n2 = (Chimera::Node*)_event.user.data2;
            log->debug("Colisao start: {} -> {}", n1->getName(), n2->getName());
        } break;
        case Chimera::KindOp::ON_COLLIDE: {
            Chimera::Node* n1 = (Chimera::Node*)_event.user.data1;
            Chimera::Node* n2 = (Chimera::Node*)_event.user.data2;
            log->debug("Colisao on: {} -> {}", n1->getName(), n2->getName());
        } break;
        case Chimera::KindOp::OFF_COLLIDE: {
            Chimera::Node* n1 = (Chimera::Node*)_event.user.data1;
            Chimera::Node* n2 = (Chimera::Node*)_event.user.data2;
            log->debug("Colisao off: {} -> {}", n1->getName(), n2->getName());
        } break;
        case Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN:
            pVideo->toggleFullScreen();
            break;
        default:
            break;
    }
}

void Game::windowEvent(const SDL_WindowEvent& _event) {

    switch (_event.event) {
        case SDL_WINDOWEVENT_ENTER:
            isPaused = false;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            isPaused = true;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pVideo->reshape(_event.data1, _event.data2);
            break;
        default:
            break;
    }
}

bool Game::paused() { return isPaused; }
