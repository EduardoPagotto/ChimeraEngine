#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/TrackHead.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/VisitParser.hpp"

Game::Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Node* _pRoot) : pCanvas(_pCanvas), pRoot(_pRoot) {}

Game::~Game() {}

void Game::joystickEvent(Chimera::Core::JoystickState* pJoy, SDL_Event* pEventSDL) {

    using namespace Chimera;
    using namespace Chimera::Core;

    // Captura joystick 0 se existir
    if (pJoy != nullptr) {

        float deadZone = 0.5f;
        float propulsaoPrincipal = 3.0f;
        float propulsaoFrontal = 1.0f;

        float yaw = pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_X, deadZone);
        float pitch = pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_Y, deadZone);
        float roll = pJoy->getAxis((uint8_t)JOY_AXIX_COD::RIGHT_X, deadZone);

        double throttle = -propulsaoPrincipal * ((1 + pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_TRIGGER, deadZone)) / 2);
        throttle = throttle - (-propulsaoFrontal * ((1 + pJoy->getAxis((uint8_t)JOY_AXIX_COD::RIGHT_TRIGGER, deadZone)) / 2));

        if (pJoy->getButtonState((uint8_t)JOY_BUTTON_COD::X) == SDL_PRESSED) {}
        if (pJoy->getButtonState((uint8_t)JOY_BUTTON_COD::B) == SDL_RELEASED) {}

        int val = pJoy->getHat(0);
        if (val & (uint8_t)SDL_HAT_UP) {}
        if (val & (uint8_t)SDL_HAT_DOWN) {}
        if (val & (uint8_t)SDL_HAT_RIGHTUP) {}
        if (val & (uint8_t)SDL_HAT_LEFT) {}
        if ((roll != 0.0) || (pitch != 0.0) || (yaw != 0.0) || (throttle != 0.0)) {}
    }
}

void Game::keboardEvent(SDL_Keycode tecla) {

    Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

    switch (tecla) {
        case SDLK_ESCAPE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_STOP, nullptr, nullptr);
            break;
        case SDLK_a:
            pCamZ->getTrackWalk()->move(Chimera::Camera_Movement::LEFT, 10);
            break;
        case SDLK_d:
            pCamZ->getTrackWalk()->move(Chimera::Camera_Movement::RIGHT, 10);
            break;
        // case SDLK_1:
        //     render3D.logToggle();
        case SDLK_F10:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseEvent(SDL_Event* pEventSDL) {

    Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

    if (Chimera::Core::MouseDevice::getButtonState(1) == SDL_PRESSED) {
        if (pEventSDL->type == SDL_MOUSEMOTION) {
            pCamZ->getTrackBall()->tracking(pEventSDL->motion.xrel, pEventSDL->motion.yrel);
        }
    } else if (Chimera::Core::MouseDevice::getButtonState(3) == SDL_PRESSED) {

        if (pEventSDL->type == SDL_MOUSEMOTION) {
            pCamZ->getTrackBall()->offSet(pEventSDL->motion.yrel);
        }
    }
}

void Game::start() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    pRoot->initializeChilds();

    pCanvas->afterStart();

    Chimera::NodeMesh* pMesh = (Chimera::NodeMesh*)pRoot->findChild("Cubo-02", true);
    renderV.pTransform = pMesh->getTransform();
    renderV.pVideo = pCanvas;
}

void Game::userEvent(const SDL_Event& _event) {
    switch (_event.user.code) {
        case Chimera::Core::EVENT_TOGGLE_FULL_SCREEN:
            pCanvas->toggleFullScreen();
            break;
        case Chimera::Core::EVENT_FLOW_START:
            this->start();
        default:
            break;
    }
}

void Game::windowEvent(const SDL_WindowEvent& _event) {
    switch (_event.event) {
        case SDL_WINDOWEVENT_ENTER:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pCanvas->reshape(_event.data1, _event.data2);
            break;
        default:
            break;
    }
}

void Game::update() {
    for (int eye = 0; eye < pCanvas->getTotEyes(); eye++) {

        pCanvas->before(eye);

        renderV.eye = eye;
        Chimera::visitParserTree(pRoot, &renderV); // dfs(root, &rv);//DFS(root);

        pCanvas->after(eye);
    }

    pCanvas->swapWindow();
}
