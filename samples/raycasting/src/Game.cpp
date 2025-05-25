#include "Game.hpp"
#include "chimera/base/event.hpp"

Game::Game() : IStateMachine("Game") {
    // init framebuffer
    canvas = ce::g_service_locator.getService<ce::ICanva>();
    // init keyboard
    keyboard = ce::g_service_locator.getService<ce::IKeyboard>();
}

Game::~Game() {
    canvas = nullptr;
    keyboard = nullptr;
}

void Game::onAttach() {

    moveSpeed = 0.05;
    rotSpeed = 0.025;

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
    using namespace ce;

    keyboard->getEvent(event);

    switch (event.type) {
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    ce::utilSendEvent(ce::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    ce::utilSendEvent(ce::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate(ce::IViewProjection& vp, const double& ts) {
    using namespace ce;

    if (keyboard->isPressed(SDLK_ESCAPE)) {
        ce::utilSendEvent(ce::EVENT_FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (keyboard->isPressed(SDLK_F10)) {
        ce::utilSendEvent(ce::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }

    if (keyboard->isPressed(SDLK_w)) {
        glm::ivec2 curr = state->pos;
        glm::ivec2 next = state->pos + state->dir * moveSpeed * 2.0f;

        if (world->data[next.x + curr.y * world->width] == 0)
            state->pos.x += state->dir.x * moveSpeed;

        if (world->data[curr.x + next.y * world->width] == 0)
            state->pos.y += state->dir.y * moveSpeed;

        return;
    }

    if (keyboard->isPressed(SDLK_s)) {
        glm::ivec2 curr = state->pos;
        glm::ivec2 next = state->pos - state->dir * moveSpeed * 2.0f;

        if (world->data[next.x + curr.y * world->width] == 0)
            state->pos.x -= state->dir.x * moveSpeed;

        if (world->data[curr.x + next.y * world->width] == 0)
            state->pos.y -= state->dir.y * moveSpeed;

        return;
    }

    if (keyboard->isPressed(SDLK_a)) {
        double oldDirX = state->dir.x;
        state->dir.x = state->dir.x * cos(rotSpeed) - state->dir.y * sin(rotSpeed);
        state->dir.y = oldDirX * sin(rotSpeed) + state->dir.y * cos(rotSpeed);
        double oldcamx = state->cam.x;
        state->cam.x = state->cam.x * cos(rotSpeed) - state->cam.y * sin(rotSpeed);
        state->cam.y = oldcamx * sin(rotSpeed) + state->cam.y * cos(rotSpeed);

        return;
    }

    if (keyboard->isPressed(SDLK_d)) {
        double oldDirX = state->dir.x;
        state->dir.x = state->dir.x * cos(-rotSpeed) - state->dir.y * sin(-rotSpeed);
        state->dir.y = oldDirX * sin(-rotSpeed) + state->dir.y * cos(-rotSpeed);
        double oldcamx = state->cam.x;
        state->cam.x = state->cam.x * cos(-rotSpeed) - state->cam.y * sin(-rotSpeed);
        state->cam.y = oldcamx * sin(-rotSpeed) + state->cam.y * cos(-rotSpeed);

        return;
    }
}

void Game::onRender() { RenderScene(*state, *world, canvas.get()); }
