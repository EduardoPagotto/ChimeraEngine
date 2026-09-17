#include "Game.hpp"
#include "chimera_base/GamePad.hpp"
#include "chimera_base/event.hpp"
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_log.h>
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

void Game::onEvent(const SDL_Event& event) {
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
}

void Game::testeGamePad() {

    using namespace ce;

    auto gp = this->inputManager->getGamepad();
    Gamepad::ButtonState bt = gp->getButtonState(0, SDL_GAMEPAD_BUTTON_NORTH);

    if (bt == Gamepad::ButtonState::Pressed) {

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Botao precionado");

    } else if (bt == Gamepad::ButtonState::Held) {

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Botao segurando");

    } else if (bt == Gamepad::ButtonState::Released) {

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Botao liberado");
    }

    glm::vec2 leftStick = gp->getLeftStick(0, player0Config);
    if (glm::length(leftStick) > 0.0F) {
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick Esquerdo -> X: %f | Y: %f", leftStick.x,
                     leftStick.y);
    }

    glm::vec2 rightStick = gp->getRightStick(0, player0Config);
    if (glm::length(rightStick) > 0.0F) {
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick Direito -> X: %f | Y: %f", rightStick.x,
                     rightStick.y);
    }

    glm::vec2 trigerStick = gp->getTriggerStick(0, player0Config);
    if (glm::length(trigerStick) > 0.0F) {
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick trigerStick -> X: %f | Y: %f", trigerStick.x,
                     trigerStick.y);
    }
}

void Game::onUpdate(const double& ts) {
    using namespace ce;

    // SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "%.3f", ts);
    testeGamePad();

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }

    if (this->inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_W)) {
        glm::ivec2 curr = state->pos;
        glm::ivec2 next = state->pos + state->dir * moveSpeed * 2.0f;

        if (world->data[next.x + curr.y * world->width] == 0)
            state->pos.x += state->dir.x * moveSpeed;

        if (world->data[curr.x + next.y * world->width] == 0)
            state->pos.y += state->dir.y * moveSpeed;
    }

    if (this->inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_S)) {
        glm::ivec2 curr = state->pos;
        glm::ivec2 next = state->pos - state->dir * moveSpeed * 2.0f;

        if (world->data[next.x + curr.y * world->width] == 0)
            state->pos.x -= state->dir.x * moveSpeed;

        if (world->data[curr.x + next.y * world->width] == 0)
            state->pos.y -= state->dir.y * moveSpeed;
    }

    if (this->inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_A)) {
        double oldDirX = state->dir.x;
        state->dir.x = state->dir.x * cos(rotSpeed) - state->dir.y * sin(rotSpeed);
        state->dir.y = oldDirX * sin(rotSpeed) + state->dir.y * cos(rotSpeed);
        double oldcamx = state->cam.x;
        state->cam.x = state->cam.x * cos(rotSpeed) - state->cam.y * sin(rotSpeed);
        state->cam.y = oldcamx * sin(rotSpeed) + state->cam.y * cos(rotSpeed);
    }

    if (this->inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_D)) {
        double oldDirX = state->dir.x;
        state->dir.x = state->dir.x * cos(-rotSpeed) - state->dir.y * sin(-rotSpeed);
        state->dir.y = oldDirX * sin(-rotSpeed) + state->dir.y * cos(-rotSpeed);
        double oldcamx = state->cam.x;
        state->cam.x = state->cam.x * cos(-rotSpeed) - state->cam.y * sin(-rotSpeed);
        state->cam.y = oldcamx * sin(-rotSpeed) + state->cam.y * cos(-rotSpeed);
    }
}

void Game::onRender() {

    // auto gFrameBuffer = canva->getPixelsCanvas()->getPixels();
    // uint64_t aTicks = SDL_GetTicks();

    // for (int i = 0, c = 0; i < canva->getHeight(); i++) {
    //     for (int j = 0; j < canva->getWidth(); j++, c++) {
    //         gFrameBuffer[c] = (uint32_t)(i * i + j * j + aTicks) | 0xff000000;
    //     }
    // }

    RenderScene(*state, *world, canva->getPixelsCanvas());
}
