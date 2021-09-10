#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"

#include <Game.hpp>

Game::Game(Chimera::CanvasGL* _pVideo) : pVideo(_pVideo) { isPaused = false; }

Game::~Game() {}

void Game::joystickEvent(Chimera::IO::JoystickState* pJoy, SDL_Event* pEventSDL) {}

void Game::keboardEvent(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::Exception(std::string(SDL_GetError()));
            }
            break;
        case SDLK_F10:
            Chimera::IO::eventsSend(Chimera::IO::TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseEvent(Chimera::IO::MouseDevice* pMouse, SDL_Event* pEventSDL) {}

void Game::start() { pVideo->initGL(); }

void Game::stop() {}

void Game::newFPS(const unsigned int& fps) {}

void Game::userEvent(const SDL_Event& _event) {
    if (_event.user.code == Chimera::IO::TOGGLE_FULL_SCREEN) {
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
    pVideo->before();
    // TODO desenhar aqui!!
    SDL_Delay(5);

    pVideo->after();
}
