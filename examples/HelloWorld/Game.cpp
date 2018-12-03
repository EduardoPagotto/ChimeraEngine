#include "Game.hpp"
#include "chimera/core/Events.hpp"
#include "chimera/core/ExceptionChimera.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/node/Transform.hpp"

Game::Game(Chimera::Video* _pVideo) : pVideo(_pVideo) { isPaused = false; }

Game::~Game() {}

void Game::joystickCapture(Chimera::JoystickManager& joy) {}

void Game::joystickStatus(Chimera::JoystickManager& joy) {}

void Game::keyCapture(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::ExceptionChimera(std::string(SDL_GetError()));
            }
            break;
        case SDLK_F10:
            Chimera::eventsSend(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {}

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
