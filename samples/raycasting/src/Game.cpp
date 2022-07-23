#include "Game.hpp"
#include "chimera/core/utils.hpp"

Game::Game(Chimera::Engine* engine) {
    // init framebuffer
    canvas = engine->getCanvas();
}

Game::~Game() {}

void Game::onAttach() {

    moveSpeed = MOVSPEED;
    rotSpeed = ROTSPEED;

    // estado de inicialização
    state = new State;
    state->pos = glm::vec2(3, 3);
    state->dir = glm::vec2(-1, 0);
    state->cam = glm::vec2(0, FOV);

    world = new World;

    if (!LoadWorld("assets/maps/raycasting_world.txt", world)) {
        printf("\nError loading world file!");
        exit(0);
    }
}

void Game::onDeatach() {}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    Chimera::utilSendEvent(Chimera::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                case SDLK_w: {
                    glm::ivec2 curr = state->pos;
                    glm::ivec2 next = state->pos + state->dir * moveSpeed * 2.0f;

                    if (world->data[next.x + curr.y * world->width] == 0)
                        state->pos.x += state->dir.x * moveSpeed;

                    if (world->data[curr.x + next.y * world->width] == 0)
                        state->pos.y += state->dir.y * moveSpeed;

                } break;
                case SDLK_s: {
                    glm::ivec2 curr = state->pos;
                    glm::ivec2 next = state->pos - state->dir * moveSpeed * 2.0f;

                    if (world->data[next.x + curr.y * world->width] == 0)
                        state->pos.x -= state->dir.x * moveSpeed;

                    if (world->data[curr.x + next.y * world->width] == 0)
                        state->pos.y -= state->dir.y * moveSpeed;

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
            }
        } break;
    }
    return true;
}

void Game::onUpdate(const double& ts) {}

void Game::onRender() { RenderScene(*state, *world, canvas); }
