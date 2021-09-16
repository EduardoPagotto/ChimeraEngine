#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"
#include <Game.hpp>

Game::Game(Chimera::CanvasGL* _pVideo) : pVideo(_pVideo) {}

Game::~Game() {}

void Game::joystickEvent(Chimera::IO::JoystickState* pJoy, SDL_Event* pEventSDL) {}

void Game::keboardEvent(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            Chimera::IO::utilSendEvent(Chimera::IO::EVENT_FLOW_STOP, nullptr, nullptr);
            break;
        case SDLK_F10:
            Chimera::IO::utilSendEvent(Chimera::IO::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseEvent(Chimera::IO::MouseDevice* pMouse, SDL_Event* pEventSDL) {}

void Game::start() {
    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
}

void Game::userEvent(const SDL_Event& _event) {
    switch (_event.user.code) {
        case Chimera::IO::EVENT_TOGGLE_FULL_SCREEN:
            pVideo->toggleFullScreen();
            break;
        case Chimera::IO::EVENT_FLOW_START:
            this->start();
        default:
            break;
    }
}

void Game::windowEvent(const SDL_WindowEvent& _event) {
    switch (_event.event) {
        case SDL_WINDOWEVENT_ENTER:
            Chimera::IO::utilSendEvent(Chimera::IO::EVENT_FLOW_RESUME, nullptr, nullptr);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Chimera::IO::utilSendEvent(Chimera::IO::EVENT_FLOW_PAUSE, nullptr, nullptr);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pVideo->reshape(_event.data1, _event.data2);
            break;
        default:
            break;
    }
}

void Game::update() {
    pVideo->before();
    // TODO desenhar aqui!!
    SDL_Delay(5);

    pVideo->after();
    pVideo->swapWindow();
}
