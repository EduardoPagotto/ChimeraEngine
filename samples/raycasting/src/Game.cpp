#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/io/utils.hpp"

Game::Game(Chimera::Core::CanvasFB* _pCanvas) : pCanvas(_pCanvas) {}

Game::~Game() {}

void Game::joystickEvent(Chimera::Core::JoystickState* pJoy, SDL_Event* pEventSDL) {}

void Game::keboardEvent(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_STOP, nullptr, nullptr);
            break;
        case SDLK_F10:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        case SDLK_w: {
            glm::ivec2 curr(state->pos.x, state->pos.y);
            glm::ivec2 next((int)(state->pos.x + state->dir.x * moveSpeed * 2), (int)(state->pos.y + state->dir.y * moveSpeed * 2));

            if (world->data[next.x + curr.y * world->width] == 0) {
                state->pos.x += state->dir.x * moveSpeed;
            }

            if (world->data[curr.x + next.y * world->width] == 0) {
                state->pos.y += state->dir.y * moveSpeed;
            }
        } break;
        case SDLK_s: {
            glm::ivec2 curr(state->pos.x, state->pos.y);
            glm::ivec2 next((int)(state->pos.x - state->dir.x * moveSpeed * 2), (int)(state->pos.y - state->dir.y * moveSpeed * 2));

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
void Game::mouseEvent(SDL_Event* pEventSDL) {}

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

    if (!LoadWorld("./samples/raycasting/world.txt", world)) {
        printf("\nError loading world file!");
        exit(0);
    }
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
    pCanvas->before();

    RenderScene(*state, *world, *frame);

    pCanvas->after();
}
