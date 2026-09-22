#include "Game.hpp"
#include "chimera_base/event.hpp"
#include "chimera_ecs/Entity.hpp"
#include "chimera_ecs/TransComponent.hpp"
#include "chimera_render/2d/Group.hpp"
#include "chimera_render/2d/Sprite.hpp"
#include "chimera_render/2d/Tile.hpp"
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_scancode.h>
// #include "chimera_base/utils.hpp"

Game::Game(std::shared_ptr<entt::registry> registry, std::shared_ptr<ce::Scene> scene)
    : registry(registry), scene(scene), pCorpoRigido(nullptr) {

    using namespace ce;

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Constructor Game");

    this->inputManager = registry->ctx().get<std::shared_ptr<ce::InputManager>>();
    this->assets = registry->ctx().get<std::shared_ptr<ce::AssetManager>>();
}

Game::~Game() {}

void Game::onEvent(const SDL_Event& event) {
    using namespace ce;

    if (event.type == CHIMERA_EVENT01) {
        switch (static_cast<EventCE>(event.user.code)) {
            case ce::EventCE::COLLIDE_START: {
                uint32_t* n1 = (uint32_t*)event.user.data1;
                uint32_t* n2 = (uint32_t*)event.user.data2;
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao start: %d -> %d", *n1, *n2);
            } break;
            case ce::EventCE::COLLIDE_ON: {
                uint32_t* n1 = (uint32_t*)event.user.data1;
                uint32_t* n2 = (uint32_t*)event.user.data2;
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao ON: %d -> %d", *n1, *n2);
            } break;
            case ce::EventCE::COLLIDE_OFF: {
                uint32_t* n1 = (uint32_t*)event.user.data1;
                uint32_t* n2 = (uint32_t*)event.user.data2;
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao OFF: %d -> %d", *n1, *n2);
            } break;
            case ce::EventCE::NEW_FPS: {
                uint32_t* pFps = (uint32_t*)event.user.data1;
                fps = *pFps;
                // glm::vec3 val1 = pCorpoRigido->getPosition();
                // sPosicaoObj = "pos:(" + std::to_string(val1.x) + "," + std::to_string(val1.y) + "," +
                // std::to_string(val1.z) + ")";
            } break;
            default:
                break;
        }
    }
}

void Game::onAttach() {

    using namespace ce;
    // Localiza objeto como o primario
    TransComponent& tc = Entity::FindComponent<TransComponent>(registry.get(), "Zoltan");

    pCorpoRigido = (Solid*)tc.trans;

    auto font = assets->getFontFromIndex(0).handle();

    lFPS = new Label("None", 0, 0, font, glm::vec4(1.0, 1.0, 1.0, 1.0));

    std::shared_ptr<Tile> tile = std::dynamic_pointer_cast<Tile>(scene->getLayes().getState("TileText"));

    Group* group = new Group(glm::translate(glm::mat4(1.0F), glm::vec3(-500.0F, 270.0F, 0.0F)));
    group->add(new Sprite(0.0F, 0.0F, 200.0F, 50.0F, glm::vec4(0, 0, 1, 0.25)));
    group->add(lFPS);
    tile->add(group);
    // tile->add(lFPS);

    glClearColor(0.F, 0.F, 0.F, 1.F); // Initialize clear color
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0F);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

void Game::onUpdate(const double& ts) {
    using namespace ce;

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }

    if (this->inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_UP)) {
        pCorpoRigido->applyForce(glm::vec3(20.0, 0.0, 0.0));
    }

    if (this->inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_DOWN)) {
        pCorpoRigido->applyForce(glm::vec3(-20.0, 0.0, 0.0));
    }

    if (this->inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_LEFT)) {
        pCorpoRigido->applyForce(glm::vec3(0.0, 20.0, 0.0));
    }

    if (this->inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_RIGHT)) {
        pCorpoRigido->applyForce(glm::vec3(0.0, -20.0, 0.0));
    }

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_A)) {
        pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, 20.0));
    }

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_S)) {
        pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, -20.0));
    }

    if (pCorpoRigido != nullptr) {
        scene->setOrigem(pCorpoRigido);
    }

    float propulsaoLRUD{5.0F};
    glm::vec3 propLateral(0.0F);

    auto gp = this->inputManager->getGamepad();
    ce::Gamepad::AxixConfig player0Config{0.18F, 0.18F, 0.18F};

    if (gp->getButtonState(0, SDL_GAMEPAD_BUTTON_DPAD_UP) == Gamepad::ButtonState::Pressed) {
        propLateral.z = propulsaoLRUD;
    }

    if (gp->getButtonState(0, SDL_GAMEPAD_BUTTON_DPAD_DOWN) == Gamepad::ButtonState::Pressed) {
        propLateral.z = -propulsaoLRUD;
    }

    if (gp->getButtonState(0, SDL_GAMEPAD_BUTTON_DPAD_LEFT) == Gamepad::ButtonState::Pressed) {
        propLateral.x = propulsaoLRUD;
    }

    if (gp->getButtonState(0, SDL_GAMEPAD_BUTTON_DPAD_RIGHT) == Gamepad::ButtonState::Pressed) {
        propLateral.x = -propulsaoLRUD;
    }

    glm::vec2 leftStick = gp->getLeftStick(0, player0Config);
    glm::vec2 rightStick = gp->getRightStick(0, player0Config);

    // int16_t deadZone = 128;
    glm::vec3 rotacao{leftStick.y, rightStick.x, leftStick.x};
    // glm::vec3 rotacao{axis16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_LEFTY), deadZone, 0x8000),
    //                   axis16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_RIGHTX), deadZone, 0x8000),
    //                   axis16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_LEFTX), deadZone, 0x8000)};

    float acc = rightStick.y;
    // float acc = axis16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_RIGHTY), deadZone, 0x8000); // ACC RIGHTX

    glm::vec3 throttle{0.0,               // X
                       -3.0F * (acc / 2), // y
                       0.0F};             // z

    if (gp->getButtonState(0, SDL_GAMEPAD_BUTTON_SOUTH) == Gamepad::ButtonState::Pressed) {
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick Botao A");
    }

    if (gp->getButtonState(0, SDL_GAMEPAD_BUTTON_EAST) == Gamepad::ButtonState::Pressed) {
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick Botao B");
    }

    glm::vec3 zero(0.0F);
    if ((rotacao != zero) || (throttle != zero) || (propLateral != zero)) {
        float torque = -0.5F;

        glm::vec3 rFinal = rotacao * torque;
        glm::vec3 vFinal = propLateral + throttle;

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Torque: %f %f %f", rFinal.x, rFinal.y, rFinal.z);
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Forca : %f %f %f", vFinal.x, vFinal.y, vFinal.z);

        pCorpoRigido->applyForce(vFinal);
        pCorpoRigido->applyTorc(rFinal);
    }
    //}

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));
}

void Game::onRender() {}
