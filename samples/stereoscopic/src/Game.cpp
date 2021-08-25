#include "Game.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TrackHead.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/VisitParser.hpp"

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Node* _pRoot) : pCanvas(_pCanvas), pRoot(_pRoot), isPaused(false) {}

Game::~Game() {}

void Game::joystickEvent(Chimera::JoystickState* pJoy) {

    using namespace Chimera;
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

void Game::keyCapture(SDL_Keycode tecla) {

    Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::Exception(std::string(SDL_GetError()));
            }
            break;
        case SDLK_a:
            pCamZ->getTrackWalk()->move(Chimera::Camera_Movement::LEFT, 10);
            break;
        case SDLK_d:
            pCamZ->getTrackWalk()->move(Chimera::Camera_Movement::RIGHT, 10);
            break;

        case SDLK_F10:
            Chimera::eventsSend(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
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

    Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

    if (estadoBotao == SDL_PRESSED) {
        if (botaoIndex == 1) {
            pCamZ->getTrackBall()->tracking(mm.xrel, mm.yrel);
        } else if (botaoIndex == 3) {
            pCamZ->getTrackBall()->offSet(mm.yrel);
        }
    }
}

void Game::start() {

    pCanvas->initGL();

    pRoot->initializeChilds();

    pCanvas->afterStart();

    Chimera::NodeMesh* pMesh = (Chimera::NodeMesh*)pRoot->findChild("Cubo-02", true);
    renderV.pTransform = pMesh->getTransform();
    renderV.pVideo = pCanvas;
}

void Game::stop() {}

void Game::newFPS(const unsigned int& fps) {}

void Game::userEvent(const SDL_Event& _event) {
    Chimera::KindOp op = (Chimera::KindOp)_event.user.code;
    if (op == Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN) {
        pCanvas->toggleFullScreen();
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
            pCanvas->reshape(_event.data1, _event.data2);
            break;
        default:
            break;
    }
}

bool Game::paused() { return isPaused; }

void Game::render() {
    for (int eye = 0; eye < pCanvas->getTotEyes(); eye++) {

        pCanvas->before(eye);

        renderV.eye = eye;
        Chimera::visitParserTree(pRoot, &renderV); // dfs(root, &rv);//DFS(root);

        pCanvas->after(eye);
    }

    pCanvas->swapWindow();
}
