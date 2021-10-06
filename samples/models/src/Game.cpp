#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/core/io/JoystickManager.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/VisitParser.hpp"

Game::Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Node* _pRoot, Chimera::PhysicsControl* _physicWorld)
    : pCanvas(_pCanvas), pRoot(_pRoot), physicWorld(_physicWorld) {

    pCorpoRigido = nullptr;
    pEmissor = nullptr;
    pOrbitalCam = nullptr;

    textoFPS = "fps: 0";
    sPosicaoObj = "pos:(,,)";

    crt.yaw = 0.0f;
    crt.pitch = 0.0f;
    crt.roll = 0.0f;
    crt.throttle = 0.0;
    crt.hat = 0;

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Constructor Game");
}

Game::~Game() { SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Destructor Game"); }

void Game::updatePos() {

    using namespace Chimera;

    float propulsaoLRUD = 5.0f;
    float torque = 0.5f;

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

void Game::joystickEvent(SDL_Event* pEventSDL) {

    using namespace Chimera;
    using namespace Chimera::Core;

    JoystickState* pJoy = nullptr;
    switch (pEventSDL->type) {
        case SDL_JOYAXISMOTION:
            pJoy = JoystickManager::select(pEventSDL->jaxis.which);
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            pJoy = JoystickManager::select(pEventSDL->jbutton.which);
            break;
        case SDL_JOYHATMOTION:
            pJoy = JoystickManager::select(pEventSDL->jhat.which);
            break;
        case SDL_JOYBALLMOTION:
            pJoy = JoystickManager::select(pEventSDL->jball.which);
            break;
    }

    // Captura joystick 0 se existir
    if (pJoy != nullptr) {

        pJoy->debug();

        float propulsaoPrincipal = 3.0f;

        crt.yaw = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_X, 32));
        crt.pitch = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_Y, 32));
        crt.roll = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::RIGHT_X, 32));

        // double throttle = 0;
        // crt.throttle = -propulsaoPrincipal * ((1 + JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_TRIGGER))) / 2);

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
}

void Game::keboardEvent(SDL_Event* pEventSDL) {

    switch (pEventSDL->key.keysym.sym) {
        case SDLK_ESCAPE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_STOP, nullptr, nullptr);
            break;
        case SDLK_F1:
            pHUD->setOn(!pHUD->isOn());
            break;
        case SDLK_F10:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
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

void Game::mouseEvent(SDL_Event* pEventSDL) {

    if (Chimera::Core::MouseDevice::getButtonState(1) == SDL_PRESSED) {
        if (pEventSDL->type == SDL_MOUSEMOTION) {
            pOrbitalCam->getTrackBall()->tracking(pEventSDL->motion.xrel, pEventSDL->motion.yrel);
        }
    } else if (Chimera::Core::MouseDevice::getButtonState(3) == SDL_PRESSED) {

        if (pEventSDL->type == SDL_MOUSEMOTION) {
            pOrbitalCam->getTrackBall()->offSet(pEventSDL->motion.yrel);
        }
    }
}

void Game::start() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    pRoot->initializeChilds();

    pCanvas->afterStart();

    // Localiza a camera
    pOrbitalCam = (Chimera::NodeCamera*)pRoot->findChild("Camera-camera", true);
    pOrbitalCam->getViewPoint()->up = glm::vec3(0, 0, -1);

    // Localiza objeto como o primario //EfeitoZoltan-mesh
    Chimera::NodeMesh* pMesh = (Chimera::NodeMesh*)pRoot->findChild("EfeitoZoltan-mesh", true);
    pCorpoRigido = (Chimera::Solid*)pMesh->getTransform();

    // Localiza o Emissor de particula
    pEmissor = (Chimera::NodeParticleEmitter*)pRoot->findChild("testeZ1", true);

    renderV.pVideo = pCanvas;
    renderV.pTransform = (Chimera::Transform*)pCorpoRigido;

    // Localiza o HUD
    pHUD = (Chimera::NodeHUD*)pRoot->findChild("HUD-Default", true);
    if (pHUD != nullptr) {
        pHUD->addText(0, glm::ivec2(350, 30), glm::vec4(0.0, 0.0, 1.0, 1.0), 1.0, &sPosicaoObj);
        pHUD->addText(0, glm::ivec2(10, 30), glm::vec4(1.0, 0.0, 0.0, 1.0), 1.0, &textoFPS);
    }
}

void Game::update() {

    physicWorld->stepSim();
    physicWorld->checkCollisions();
    this->updatePos();

    for (int eye = 0; eye < pCanvas->getTotEyes(); eye++) {

        pCanvas->before(eye);

        renderV.eye = eye;
        Chimera::visitParserTree(pRoot, &renderV); // dfs(root, &rv);//DFS(root);

        pCanvas->after(eye);
    }

    pCanvas->swapWindow();
}

void Game::userEvent(const SDL_Event& _event) {

    Chimera::Node* n1 = (Chimera::Node*)_event.user.data1;
    Chimera::Node* n2 = (Chimera::Node*)_event.user.data2;

    switch (_event.user.code) {
        case Chimera::Core::EVENT_COLLIDE_START: {
            Chimera::Node* n1 = (Chimera::Node*)_event.user.data1;
            Chimera::Node* n2 = (Chimera::Node*)_event.user.data2;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao start: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
        } break;
        case Chimera::Core::EVENT_COLLIDE_ON: {
            Chimera::Node* n1 = (Chimera::Node*)_event.user.data1;
            Chimera::Node* n2 = (Chimera::Node*)_event.user.data2;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao on: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
        } break;
        case Chimera::Core::EVENT_COLLIDE_OFF: {
            Chimera::Node* n1 = (Chimera::Node*)_event.user.data1;
            Chimera::Node* n2 = (Chimera::Node*)_event.user.data2;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao off: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
        } break;
        case Chimera::Core::EVENT_TOGGLE_FULL_SCREEN:
            pCanvas->toggleFullScreen();
            break;
        case Chimera::Core::EVENT_NEW_FPS: {
            uint32_t* fps = (uint32_t*)_event.user.data1;
            glm::vec3 val1 = pCorpoRigido->getPosition();
            sPosicaoObj = "pos:(" + std::to_string(val1.x) + "," + std::to_string(val1.y) + "," + std::to_string(val1.z) + ")";
            textoFPS = "fps: " + std::to_string(*fps) + std::string(" Periodo: ") + std::to_string(physicWorld->getLastPeriod());
        } break;
        case Chimera::Core::EVENT_FLOW_START:
            this->start();
        default:
            break;
    }
}

void Game::windowEvent(SDL_Event* pEventSDL) {
    switch (pEventSDL->window.event) {
        case SDL_WINDOWEVENT_ENTER:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pCanvas->reshape(pEventSDL->window.data1, pEventSDL->window.data2);
            break;
        default:
            break;
    }
}
