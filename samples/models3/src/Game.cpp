#include "Game.hpp"
#include "chimera/base/event.hpp"
#include "chimera/base/utils.hpp"
#include "chimera/core/gl/FontMng.hpp"
#include "chimera/ecs/TransComponent.hpp"
#include "chimera/render/2d/Group.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include "chimera/render/2d/Tile.hpp"

Game::Game(ce::Scene* scene) : IStateMachine("Game"), scene(scene), pCorpoRigido(nullptr) {
    using namespace ce;
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Constructor Game");

    gameControl = g_service_locator.getService<GamePad>();
    mouse = g_service_locator.getService<Mouse>();
}

Game::~Game() {
    gameControl = nullptr;
    mouse = nullptr;
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace ce;

    gameControl->getEvent(event);
    mouse->getEvent(event);

    switch (event.type) {
        case SDL_EVENT_USER: {
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

        } break;
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_ESCAPE:
                    sendChimeraEvent(EventCE::FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    sendChimeraEvent(EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                case SDLK_UP:
                    pCorpoRigido->applyForce(glm::vec3(10.0, 0.0, 0.0));
                    break;
                case SDLK_DOWN:
                    pCorpoRigido->applyForce(glm::vec3(-10.0, 0.0, 0.0));
                    break;
                case SDLK_LEFT:
                    pCorpoRigido->applyForce(glm::vec3(0.0, 10.0, 0.0));
                    break;
                case SDLK_RIGHT:
                    pCorpoRigido->applyForce(glm::vec3(0.0, -10.0, 0.0));
                    break;
                case SDLK_A:
                    pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, 10.0));
                    break;
                case SDLK_S:
                    pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, -10.0));
                    break;
                default:
                    break;
            }
        } break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            sendChimeraEvent(EventCE::FLOW_RESUME, nullptr, nullptr); // isPaused = false;
            break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            sendChimeraEvent(EventCE::FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
            break;
    }
    return true;
}

void Game::onAttach() {

    using namespace ce;
    // Localiza objeto como o primario
    TransComponent& tc = g_registry.findComponent<TransComponent>("Zoltan");
    pCorpoRigido = (Solid*)tc.trans;

    auto fontMng = g_service_locator.getService<FontMng>();
    auto font = fontMng->getIndex(0);
    lFPS = new Label("None", 0, 0, font, glm::vec4(1.0, 1.0, 1.0, 1.0));
    Tile* tile = (Tile*)scene->getLayes().getState("TileText");

    Group* group = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, 270.0f, 0.0f)));
    group->add(new Sprite(0.0f, 0.0f, 200.0f, 50.0f, glm::vec4(0, 0, 1, 0.25)));
    group->add(lFPS);
    tile->add(group);
    // tile->add(lFPS);

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

void Game::onUpdate(const double& ts) {
    using namespace ce;

    if (pCorpoRigido)
        scene->setOrigem(pCorpoRigido);

    if (SDL_Gamepad* pJoy = gameControl->get(0); pJoy != nullptr) {

        float propulsaoLRUD{5.0f};
        glm::vec3 propLateral(0.0f);
        if (SDL_GetGamepadButton(pJoy, SDL_GAMEPAD_BUTTON_DPAD_UP))
            propLateral.z = propulsaoLRUD;

        if (SDL_GetGamepadButton(pJoy, SDL_GAMEPAD_BUTTON_DPAD_DOWN))
            propLateral.z = -propulsaoLRUD;

        if (SDL_GetGamepadButton(pJoy, SDL_GAMEPAD_BUTTON_DPAD_LEFT))
            propLateral.x = propulsaoLRUD;

        if (SDL_GetGamepadButton(pJoy, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
            propLateral.x = -propulsaoLRUD;

        int16_t deadZone = 128;
        glm::vec3 rotacao{scale16(dead16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_LEFTY), deadZone), 0x8000),
                          scale16(dead16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_RIGHTX), deadZone), 0x8000),
                          scale16(dead16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_LEFTX), deadZone), 0x8000)};

        float acc = scale16(dead16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_RIGHTY), deadZone),
                            0x8000); // ACC RIGHTX

        glm::vec3 throttle{0.0,               // X
                           -3.0f * (acc / 2), // y
                           0.0f};             // z

        if (SDL_GetGamepadButton(pJoy, SDL_GAMEPAD_BUTTON_SOUTH))
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick Botao A");

        if (SDL_GetGamepadButton(pJoy, SDL_GAMEPAD_BUTTON_EAST))
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick Botao B");

        glm::vec3 zero(0.0f);
        if ((rotacao != zero) || (throttle != zero) || (propLateral != zero)) {
            float torque = -0.5f;

            glm::vec3 rFinal = rotacao * torque;
            glm::vec3 vFinal = propLateral + throttle;

            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Torque: %f %f %f", rFinal.x, rFinal.y, rFinal.z);
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Forca : %f %f %f", vFinal.x, vFinal.y, vFinal.z);

            pCorpoRigido->applyForce(vFinal);
            pCorpoRigido->applyTorc(rFinal);
        }
    }

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));
}

void Game::onRender() {}
