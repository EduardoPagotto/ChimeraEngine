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
            glm::ivec2 curr(state->pos.x, state->pos.y);
            glm::ivec2 next((int)(state->pos.x + state->dir.x * moveSpeed * 2),
                            (int)(state->pos.y + state->dir.y * moveSpeed * 2));

            if (world->data[next.x + curr.y * world->width] == 0) {
                state->pos.x += state->dir.x * moveSpeed;
            }

            if (world->data[curr.x + next.y * world->width] == 0) {
                state->pos.y += state->dir.y * moveSpeed;
            }
        } break;
        case SDLK_s: {
            glm::ivec2 curr(state->pos.x, state->pos.y);
            glm::ivec2 next((int)(state->pos.x - state->dir.x * moveSpeed * 2),
                            (int)(state->pos.y - state->dir.y * moveSpeed * 2));

            if (world->data[next.x + curr.y * world->width] == 0) {
                state->pos.x -= state->dir.x * moveSpeed;
            }
            if (world->data[curr.x + next.y * world->width] == 0) {
                state->pos.y -= state->dir.y * moveSpeed;
            }
        } break;
        case SDLK_a: {
            double oldDirX = state->dir.x;
            state->dir.x = state->dir.x * cos(rotSpeed) - state->dir.y * sin(rotSpeed);
            state->dir.y = oldDirX * sin(rotSpeed) + state->dir.y * cos(rotSpeed);
            double oldcamx = state->cam.x;
            state->cam.x = state->cam.x * cos(rotSpeed) - state->cam.y * sin(rotSpeed);
            state->cam.y = oldcamx * sin(rotSpeed) + state->cam.y * cos(rotSpeed);
        } break;
        case SDLK_d: {
            double oldDirX = state->dir.x;
            state->dir.x = state->dir.x * cos(-rotSpeed) - state->dir.y * sin(-rotSpeed);
            state->dir.y = oldDirX * sin(-rotSpeed) + state->dir.y * cos(-rotSpeed);
            double oldcamx = state->cam.x;
            state->cam.x = state->cam.x * cos(-rotSpeed) - state->cam.y * sin(-rotSpeed);
            state->cam.y = oldcamx * sin(-rotSpeed) + state->cam.y * cos(-rotSpeed);
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
    frame->data = pCanvas->getPixels();
    frame->width = pCanvas->getWidth();
    frame->height = pCanvas->getHeight();

    // estado de inicialização
    state = new State;
    state->pos = glm::vec2(3, 3);
    state->dir = glm::vec2(-1, 0);
    state->cam = glm::vec2(0, FOV);

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
