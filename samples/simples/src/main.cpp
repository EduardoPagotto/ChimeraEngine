#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/ServiceLocator.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/device/GameController.hpp"
#include "chimera/core/device/Joystick.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/core/visible/ShaderMng.hpp"
#include "chimera/render/scene/Scene.hpp"

Game::Game(std::shared_ptr<ServiceLocator> sl) : IStateMachine("Game"), serviceLoc(sl) { mouse = sl->getService<ce::IMouse>(); }
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
        auto sl = std::make_shared<ServiceLocator>();
        sl->registerService(reg);
        sl->registerService(std::make_shared<CanvasGL>("BSP Tree", 1800, 600, false));
        sl->registerService(std::make_shared<Mouse>());
        sl->registerService(std::make_shared<Keyboard>());
        sl->registerService(std::make_shared<Joystick>());
        sl->registerService(std::make_shared<GameController>());
        sl->registerService(std::make_shared<ViewProjection>(0.5f)); // View projection
        sl->registerService(std::make_shared<ShaderMng>());
        sl->registerService(std::make_shared<TextureMng>());

        // Engine
        Engine engine(sl);

        ColladaDom dom = loadFileCollada("./samples/simples/level.xml");
        colladaRegistryLoad(dom, sl);

        Scene scene(sl);
        Game game(sl);

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
