#include "Game.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/2d/Group.hpp"
#include "chimera/render/2d/Sprite.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Scene& scene) : IStateMachine("Game"), pCorpoRigido(nullptr), scene(&scene) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Constructor Game");
    registry = Chimera::Singleton<Chimera::Registry>::get();
    gameControl = Chimera::Singleton<Chimera::GameController>::get();
    mouse = Chimera::Singleton<Chimera::MouseDevice>::get();
}

Game::~Game() {
    Chimera::Singleton<Chimera::Registry>::release();
    Chimera::Singleton<Chimera::GameController>::release();
    Chimera::Singleton<Chimera::MouseDevice>::release();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Destructor Game");
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    gameControl->getEvent(event);
    mouse->getEvent(event);

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case Chimera::EVENT_COLLIDE_START: {
                    uint32_t* n1 = (uint32_t*)event.user.data1;
                    uint32_t* n2 = (uint32_t*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao start: %d -> %d", *n1, *n2);
                } break;
                case Chimera::EVENT_COLLIDE_ON: {
                    uint32_t* n1 = (uint32_t*)event.user.data1;
                    uint32_t* n2 = (uint32_t*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao ON: %d -> %d", *n1, *n2);
                } break;
                case Chimera::EVENT_COLLIDE_OFF: {
                    uint32_t* n1 = (uint32_t*)event.user.data1;
                    uint32_t* n2 = (uint32_t*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao OFF: %d -> %d", *n1, *n2);
                } break;
                case Chimera::EVENT_NEW_FPS: {
                    uint32_t* pFps = (uint32_t*)event.user.data1;
                    fps = *pFps;
                    // glm::vec3 val1 = pCorpoRigido->getPosition();
                    // sPosicaoObj = "pos:(" + std::to_string(val1.x) + "," + std::to_string(val1.y) + "," + std::to_string(val1.z) + ")";
                } break;
                default:
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    utilSendEvent(EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    utilSendEvent(EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
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
                case SDLK_a:
                    pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, 10.0));
                    break;
                case SDLK_s:
                    pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, -10.0));
                    break;
                default:
                    break;
            }
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    utilSendEvent(EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    utilSendEvent(EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
            }
        } break;
    }
    return true;
}

void Game::onAttach() {

    using namespace Chimera;
    // Localiza objeto como o primario
    TransComponent& tc = registry->findComponent<TransComponent>("Zoltan");
    pCorpoRigido = (Solid*)tc.trans;

    lFPS = new Label("None", 0, 0, glm::vec4(1.0, 1.0, 1.0, 1.0));
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

void Game::onUpdate(Chimera::ViewProjection& vp, const double& ts) {
    using namespace Chimera;

    if (pCorpoRigido)
        scene->setOrigem(pCorpoRigido);

    if (SDL_GameController* pJoy = gameControl->get(0); pJoy != nullptr) {

        float propulsaoLRUD{5.0f};
        glm::vec3 propLateral(0.0f);
        if (SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_DPAD_UP) == SDL_PRESSED)
            propLateral.z = propulsaoLRUD;

        if (SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == SDL_PRESSED)
            propLateral.z = -propulsaoLRUD;

        if (SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == SDL_PRESSED)
            propLateral.x = propulsaoLRUD;

        if (SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == SDL_PRESSED)
            propLateral.x = -propulsaoLRUD;

        int16_t deadZone = 128;
        glm::vec3 rotacao{scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_LEFTY), deadZone), 0x8000),  // pitch LEFTY
                          scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_RIGHTX), deadZone), 0x8000), // roll LEFTX
                          scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_LEFTX), deadZone), 0x8000)}; // yaw RIGHTY

        float acc = scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_RIGHTY), deadZone), 0x8000); // ACC RIGHTX
        glm::vec3 throttle{0.0,                                                                                     // X
                           -3.0f * (acc / 2),                                                                       // y
                           0.0f};                                                                                   // z

        if (SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_A) == SDL_PRESSED)
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick Botao A");

        if (SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_B) == SDL_PRESSED)
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