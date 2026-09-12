#include "Game.hpp"
#include "chimera_base/event.hpp"
#include <format>

Game::Game(std::shared_ptr<entt::registry> registry, std::shared_ptr<ce::CanvaFB> canva)
    : registry(registry), canva(canva) {

    this->inputManager = registry->ctx().get<std::shared_ptr<ce::InputManager>>();
}

Game::~Game() {}

std::string Game::getName() const { return "GAME"; }

void Game::onAttach() {

    moveSpeed = 0.05;
    rotSpeed = 0.025;

    // estado de inicialização
    state = new State;
    state->pos = glm::vec2(3, 3);
    state->dir = glm::vec2(-1, 0);
    state->cam = glm::vec2(0, fov);

    world = new World;

    const char* file = "assets/maps/raycasting_world.txt";

    if (!LoadWorld(file, world)) {
        throw std::runtime_error(std::format("File not found: {}", file));
    }
}

void Game::onDeatach() {}

bool Game::onEvent(const SDL_Event& event) {
    // using namespace ce;

    // keyboard->getEvent(event);

    // switch (event.type) {
    //     case SDL_EVENT_WINDOW_MOUSE_ENTER:
    //         ce::sendChimeraEvent(ce::EventCE::FLOW_RESUME, nullptr, nullptr); // isPaused = false;
    //         break;
    //     case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    //         ce::sendChimeraEvent(ce::EventCE::FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
    //         break;
    // }
    return false;
}

void Game::onUpdate(const double& ts) {
    using namespace ce;

    if (this->inputManager->keyboard->isPressed(SDLK_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->inputManager->keyboard->isPressed(SDLK_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }

    if (this->inputManager->keyboard->isPressed(SDLK_W)) {
        glm::ivec2 curr = state->pos;
        glm::ivec2 next = state->pos + state->dir * moveSpeed * 2.0f;

        if (world->data[next.x + curr.y * world->width] == 0)
            state->pos.x += state->dir.x * moveSpeed;

        if (world->data[curr.x + next.y * world->width] == 0)
            state->pos.y += state->dir.y * moveSpeed;

        return;
    }

    if (this->inputManager->keyboard->isPressed(SDLK_S)) {
        glm::ivec2 curr = state->pos;
        glm::ivec2 next = state->pos - state->dir * moveSpeed * 2.0f;

        if (world->data[next.x + curr.y * world->width] == 0)
            state->pos.x -= state->dir.x * moveSpeed;

        if (world->data[curr.x + next.y * world->width] == 0)
            state->pos.y -= state->dir.y * moveSpeed;

        return;
    }

    if (this->inputManager->keyboard->isPressed(SDLK_A)) {
        double oldDirX = state->dir.x;
        state->dir.x = state->dir.x * cos(rotSpeed) - state->dir.y * sin(rotSpeed);
        state->dir.y = oldDirX * sin(rotSpeed) + state->dir.y * cos(rotSpeed);
        double oldcamx = state->cam.x;
        state->cam.x = state->cam.x * cos(rotSpeed) - state->cam.y * sin(rotSpeed);
        state->cam.y = oldcamx * sin(rotSpeed) + state->cam.y * cos(rotSpeed);

        return;
    }

    if (this->inputManager->keyboard->isPressed(SDLK_D)) {
        double oldDirX = state->dir.x;
        state->dir.x = state->dir.x * cos(-rotSpeed) - state->dir.y * sin(-rotSpeed);
        state->dir.y = oldDirX * sin(-rotSpeed) + state->dir.y * cos(-rotSpeed);
        double oldcamx = state->cam.x;
        state->cam.x = state->cam.x * cos(-rotSpeed) - state->cam.y * sin(-rotSpeed);
        state->cam.y = oldcamx * sin(-rotSpeed) + state->cam.y * cos(-rotSpeed);

        return;
    }
}

void Game::onRender() {

    // int* gFrameBuffer = (int*)canva.get()->getPixels();
    // uint64_t aTicks = SDL_GetTicks();

    // for (int i = 0, c = 0; i < canva->getHeight(); i++) {
    //     for (int j = 0; j < canva->getWidth(); j++, c++) {
    //         gFrameBuffer[c] = (int)(i * i + j * j + aTicks) | 0xff000000;
    //     }
    // }

    RenderScene(*state, *world, canva->getPixelsCanvas());
}
