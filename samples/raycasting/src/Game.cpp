#include "Game.hpp"
#include "chimera_base/GamePad.hpp"
#include "chimera_base/ICanva.hpp"
#include "chimera_base/event.hpp"
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_log.h>
#include <format>
#include <stdexcept>

Game::Game(std::shared_ptr<entt::registry> registry) : registry_(registry) {

    this->canva_ = std::dynamic_pointer_cast<ce::CanvaFB>(registry->ctx().get<std::shared_ptr<ce::ICanva>>());
    if (this->canva_ == nullptr) {
        throw std::runtime_error("Canva not found in CTX");
    }

    this->input_manager_ = registry->ctx().get<std::shared_ptr<ce::InputManager>>();
}

Game::~Game() {}

std::string Game::get_name() const { return "GAME"; }

void Game::on_attach() {

    move_speed_ = 0.05;
    rot_speed_ = 0.025;

    // estado de inicialização
    state_ = new State;
    state_->pos = glm::vec2(3, 3);
    state_->dir = glm::vec2(-1, 0);
    state_->cam = glm::vec2(0, fov);

    world_ = new World;

    const char* file = "assets/maps/raycasting_world.txt";

    if (!LoadWorld(file, world_)) {
        throw std::runtime_error(std::format("File not found: {}", file));
    }
}

void Game::on_deatach() {}

void Game::on_event(const SDL_Event& event) {
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

void Game::teste_game_pad() {

    using namespace ce;

    auto gp = this->input_manager_->get_gamepad();
    Gamepad::ButtonState bt = gp->get_button_state(0, SDL_GAMEPAD_BUTTON_NORTH);

    if (bt == Gamepad::ButtonState::Pressed) {

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Botao precionado");

    } else if (bt == Gamepad::ButtonState::Held) {

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Botao segurando");

    } else if (bt == Gamepad::ButtonState::Released) {

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Botao liberado");
    }

    glm::vec2 left_stick = gp->get_left_stick(0, player0_config_);
    if (glm::length(left_stick) > 0.0F) {
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick Esquerdo -> X: %f | Y: %f", left_stick.x,
                     left_stick.y);
    }

    glm::vec2 right_stick = gp->get_right_stick(0, player0_config_);
    if (glm::length(right_stick) > 0.0F) {
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick Direito -> X: %f | Y: %f", right_stick.x,
                     right_stick.y);
    }

    glm::vec2 triger_stick = gp->get_trigger_stick(0, player0_config_);
    if (glm::length(triger_stick) > 0.0F) {
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick trigerStick -> X: %f | Y: %f", triger_stick.x,
                     triger_stick.y);
    }
}

void Game::on_update(const double& ts) {
    using namespace ce;

    // SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "%.3f", ts);
    teste_game_pad();

    if (this->input_manager_->get_keyboard()->is_key_pressed(SDL_SCANCODE_ESCAPE)) {
        send_chimera_event(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->input_manager_->get_keyboard()->is_key_pressed(SDL_SCANCODE_F1)) {
        send_chimera_event(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }

    if (this->input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_W)) {
        glm::ivec2 curr = state_->pos;
        glm::ivec2 next = state_->pos + state_->dir * move_speed_ * 2.0f;

        if (world_->data[next.x + curr.y * world_->width] == 0)
            state_->pos.x += state_->dir.x * move_speed_;

        if (world_->data[curr.x + next.y * world_->width] == 0)
            state_->pos.y += state_->dir.y * move_speed_;
    }

    if (this->input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_S)) {
        glm::ivec2 curr = state_->pos;
        glm::ivec2 next = state_->pos - state_->dir * move_speed_ * 2.0f;

        if (world_->data[next.x + curr.y * world_->width] == 0)
            state_->pos.x -= state_->dir.x * move_speed_;

        if (world_->data[curr.x + next.y * world_->width] == 0)
            state_->pos.y -= state_->dir.y * move_speed_;
    }

    if (this->input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_A)) {
        double old_dir_x = state_->dir.x;
        state_->dir.x = state_->dir.x * cos(rot_speed_) - state_->dir.y * sin(rot_speed_);
        state_->dir.y = old_dir_x * sin(rot_speed_) + state_->dir.y * cos(rot_speed_);
        double oldcamx = state_->cam.x;
        state_->cam.x = state_->cam.x * cos(rot_speed_) - state_->cam.y * sin(rot_speed_);
        state_->cam.y = oldcamx * sin(rot_speed_) + state_->cam.y * cos(rot_speed_);
    }

    if (this->input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_D)) {
        double old_dir_x = state_->dir.x;
        state_->dir.x = state_->dir.x * cos(-rot_speed_) - state_->dir.y * sin(-rot_speed_);
        state_->dir.y = old_dir_x * sin(-rot_speed_) + state_->dir.y * cos(-rot_speed_);
        double oldcamx = state_->cam.x;
        state_->cam.x = state_->cam.x * cos(-rot_speed_) - state_->cam.y * sin(-rot_speed_);
        state_->cam.y = oldcamx * sin(-rot_speed_) + state_->cam.y * cos(-rot_speed_);
    }
}

void Game::on_render() {

    // auto gFrameBuffer = canva->getPixelsCanvas()->getPixels();
    // uint64_t aTicks = SDL_GetTicks();

    // for (int i = 0, c = 0; i < canva->getHeight(); i++) {
    //     for (int j = 0; j < canva->getWidth(); j++, c++) {
    //         gFrameBuffer[c] = (uint32_t)(i * i + j * j + aTicks) | 0xff000000;
    //     }
    // }

    RenderScene(*state_, *world_, canva_->pixels_canvas());
}
