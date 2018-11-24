#include "Game.hpp"
#include "ExceptionSDL.hpp"

#include "GameClient.hpp"
#include "OpenGLDefs.hpp"
#include "Transform.hpp"

Game::Game(Chimera::Video* _pVideo) : pVideo(_pVideo) { isPaused = false; }

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

        double throttle =
            -propulsaoPrincipal *
            ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_TRIGGER, deadZone)) / 2);
        throttle =
            throttle -
            (-propulsaoFrontal *
             ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_TRIGGER, deadZone)) / 2));

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

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::ExceptionSDL(Chimera::ExceptionCode::CLOSE,
                                            std::string(SDL_GetError()));
            }
            break;
        case SDLK_F10:
            Chimera::sendMessage(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr,
                                 nullptr);
            break;
        case SDLK_F1:
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_LEFT:
        case SDLK_RIGHT:
        case SDLK_a:
        case SDLK_s:
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
            // pOrbitalCam->trackBall(mm.yrel, mm.xrel, 0);
        } else if (botaoIndex == 2) {
            // pOrbitalCam->trackBall(0, 0, mm.yrel);
        }
    }
}

void Game::start() { pVideo->initGL(); }

void Game::stop() {}

void Game::newFPS(const unsigned int& fps) {}

void Game::userEvent(const SDL_Event& _event) {
    Chimera::KindOp op = (Chimera::KindOp)_event.user.code;
    if (op == Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN) {
        pVideo->toggleFullScreen();
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

void Game::render() {

    pVideo->initDraw();
    // TODO desenhar aqui!!

    pVideo->endDraw();
}
