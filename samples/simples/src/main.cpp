#include "Game.hpp"
#include "chimera/base/GameController.hpp"
#include "chimera/base/Joystick.hpp"
#include "chimera/base/Keyboard.hpp"
#include "chimera/base/ServiceLocator.hpp"
#include "chimera/collada/colladaLoad.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/gl/CanvasGL.hpp"
#include "chimera/core/gl/ShaderMng.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/scene/Scene.hpp"

Game::Game() : IStateMachine("Game") { mouse = ce::g_service_locator.getService<ce::IMouse>(); }
Game::~Game() { mouse = nullptr; }

void Game::onAttach() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
    // Habilita o depth buffer/culling face
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

bool Game::onEvent(const SDL_Event& event) {
    using namespace ce;

    mouse->getEvent(event);

    switch (event.type) {
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    utilSendEvent(EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F10:
                    utilSendEvent(EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                default:
                    break;
            }
        } break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
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

void Game::onUpdate(ce::IViewProjection& vp, const double& ts) {}

void Game::onRender() {}

int main(int argn, char** argv) {
    using namespace ce;
    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simnples Iniciado");

        // Registry to entt
        auto reg = std::make_shared<Registry>();

        // Services shared inside all parts
        // Canvas, Mouse, keyboard, Joystick, gamepad, view's
        g_service_locator.registerService(std::make_shared<CanvasGL>("BSP Tree", 1800, 600, false));
        g_service_locator.registerService(std::make_shared<Mouse>());
        g_service_locator.registerService(std::make_shared<Keyboard>());
        g_service_locator.registerService(std::make_shared<Joystick>());
        g_service_locator.registerService(std::make_shared<GameController>());
        g_service_locator.registerService(std::make_shared<ViewProjection>(0.5f)); // View projection
        g_service_locator.registerService(std::make_shared<ShaderMng>());
        g_service_locator.registerService(std::make_shared<TextureMng>());

        // Engine
        Engine engine;

        ColladaDom dom = loadFileCollada("./samples/simples/level.xml");
        colladaRegistryLoad(dom);

        Scene scene;
        Game game;

        engine.getStack().pushState(&scene);
        engine.getStack().pushState(&game);

        Collada::destroy(); // clean loader

        engine.run();
        SDL_Log("Loop de Game encerrado!!!!");
        return 0;

    } catch (const std::string& ex) {
        // fail 3
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // Fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
