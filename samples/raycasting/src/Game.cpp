#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/io/utils.hpp"

Game::Game(Chimera::CanvasFB* _pCanvas) : pCanvas(_pCanvas) {}

Game::~Game() {}

void Game::onStart() {

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

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case Chimera::EVENT_TOGGLE_FULL_SCREEN:
                    pCanvas->toggleFullScreen();
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    Chimera::utilSendEvent(Chimera::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
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
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    pCanvas->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate() {
    pCanvas->before();

    RenderScene(*state, *world, *frame);

    pCanvas->after();
}
