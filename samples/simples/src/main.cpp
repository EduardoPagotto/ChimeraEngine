#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/utils.hpp"

Game::Game(Chimera::Scene& scene) : IStateMachine("Game"), scene(&scene) {}

Game::~Game() {}

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
    using namespace Chimera;

    MouseDevice::getEvent(event);

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

void Game::onUpdate(Chimera::ViewProjection& vp, const double& ts) {}

void Game::onRender() {}

int main(int argn, char** argv) {
    using namespace Chimera;
    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simnples Iniciado");

        RegistryManager::create();

        Engine engine(new CanvasGL("BSP Tree", 1800, 600, false), 0.0f);

        ColladaDom dom = loadFileCollada("./samples/simples/level.xml");
        colladaRegistryLoad(dom);

        engine.init();

        Scene scene;

        Game* game = new Game(scene);

        engine.getStack().pushState(&scene);
        engine.getStack().pushState(game);

        Collada::destroy(); // clean loader

        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;

        SDL_Log("AppShader finalizado com sucesso");
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
