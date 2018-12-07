#include "Game.hpp"
#include "chimera/core/Events.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/node/Transform.hpp"

Game::Game(Chimera::CanvasFB* _pCanvas) : pCanvas(_pCanvas) { isPaused = false; }

Game::~Game() {}

void Game::joystickCapture(Chimera::JoystickManager& joy) {}

void Game::joystickStatus(Chimera::JoystickManager& joy) {}

void Game::keyCapture(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::Exception(std::string(SDL_GetError()));
            }
            break;
        case SDLK_F10:
            Chimera::eventsSend(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        case SDLK_w: {
            int currX = (int)state->posx;
            int currY = (int)state->posy;
            int nextX = (int)(state->posx + state->dirx * moveSpeed * 2);
            int nextY = (int)(state->posy + state->diry * moveSpeed * 2);

            if (world->data[nextX + currY * world->width] == 0) {
                state->posx += state->dirx * moveSpeed;
            }

            if (world->data[currX + nextY * world->width] == 0) {
                state->posy += state->diry * moveSpeed;
            }
        } break;
        case SDLK_s: {
            int currX = (int)state->posx;
            int currY = (int)state->posy;
            int nextX = (int)(state->posx - state->dirx * moveSpeed * 2);
            int nextY = (int)(state->posy - state->diry * moveSpeed * 2);

            if (world->data[nextX + currY * world->width] == 0) {
                state->posx -= state->dirx * moveSpeed;
            }
            if (world->data[currX + nextY * world->width] == 0) {
                state->posy -= state->diry * moveSpeed;
            }
        } break;
        case SDLK_a: {
            double oldDirX = state->dirx;
            state->dirx = state->dirx * cos(rotSpeed) - state->diry * sin(rotSpeed);
            state->diry = oldDirX * sin(rotSpeed) + state->diry * cos(rotSpeed);
            double oldcamx = state->camx;
            state->camx = state->camx * cos(rotSpeed) - state->camy * sin(rotSpeed);
            state->camy = oldcamx * sin(rotSpeed) + state->camy * cos(rotSpeed);
        } break;
        case SDLK_d: {
            double oldDirX = state->dirx;
            state->dirx = state->dirx * cos(-rotSpeed) - state->diry * sin(-rotSpeed);
            state->diry = oldDirX * sin(-rotSpeed) + state->diry * cos(-rotSpeed);
            double oldcamx = state->camx;
            state->camx = state->camx * cos(-rotSpeed) - state->camy * sin(-rotSpeed);
            state->camy = oldcamx * sin(-rotSpeed) + state->camy * cos(-rotSpeed);
        } break;
        default:
            break;
    }
}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {}

void Game::start() {

    moveSpeed = MOVSPEED;
    rotSpeed = ROTSPEED;

    // init framebuffer
    frame = new Frame;
    frame->data = pCanvas->getPixels(); // new TrueColorPixel[SCREEN_W * SCREEN_H];
    frame->width = pCanvas->getWidth();
    frame->height = pCanvas->getHeight();

    // estado de inicialização
    state = new State;
    state->posx = 3;
    state->posy = 3;
    state->dirx = -1;
    state->diry = 0;
    state->camx = 0;
    state->camy = FOV;

    world = new World;

    if (!LoadWorld("./examples/simpleRayCasting/world.txt", world)) {
        printf("\nError loading world file!");
        exit(0);
    }
}

void Game::stop() {
    // delete world;
    // delete state;
    // delete frame;
}

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
    pCanvas->before();

    RenderScene(*state, *world, *frame);

    pCanvas->after();
}
