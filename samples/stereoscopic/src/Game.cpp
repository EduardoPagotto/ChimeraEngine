#include "Game.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TrackHead.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/node/Camera.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/NodeParse.hpp"

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Node* _pRoot) : pCanvas(_pCanvas), pRoot(_pRoot), isPaused(false) {}

Game::~Game() {}

void Game::joystickCapture(Chimera::JoystickManager& joy) {}

void Game::joystickStatus(Chimera::JoystickManager& joy) {

    using namespace Chimera;
    // Captura joystick 0 se existir
    JoystickState* joystick = joy.getJoystickState(0);
    if (joystick != nullptr) {

        float deadZone = 0.5f;
        float propulsaoPrincipal = 3.0f;
        float propulsaoFrontal = 1.0f;

        float yaw = joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_X, deadZone);
        float pitch = joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_Y, deadZone);
        float roll = joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_X, deadZone);

        double throttle = -propulsaoPrincipal * ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_TRIGGER, deadZone)) / 2);
        throttle =
            throttle - (-propulsaoFrontal * ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_TRIGGER, deadZone)) / 2));

        if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::X) == true) {}
        if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::B) == true) {}

        int val = joystick->Hat(0);
        if (val & (uint8_t)JOY_PAD_COD::UP) {}
        if (val & (uint8_t)JOY_PAD_COD::DOWN) {}
        if (val & (uint8_t)JOY_PAD_COD::RIGHT) {}
        if (val & (uint8_t)JOY_PAD_COD::LEFT) {}
        if ((roll != 0.0) || (pitch != 0.0) || (yaw != 0.0) || (throttle != 0.0)) {}
    }
}

void Game::keyCapture(SDL_Keycode tecla) {

    Chimera::Camera* pCamZ = (Chimera::Camera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

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

    Chimera::Camera* pCamZ = (Chimera::Camera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

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

    Chimera::Mesh* pMesh = (Chimera::Mesh*)pRoot->findChild("Cubo-02", true);
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
        Chimera::NodeParse::tree(pRoot, &renderV); // dfs(root, &rv);//DFS(root);

        pCanvas->after(eye);
    }

    pCanvas->swapWindow();
}
