#include "Game.hpp"
#include "chimera/collada/colladaLoad.hpp"
#include "chimera/core/CanvasFB.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/Keyboard.hpp"
#include "chimera/core/Registry.hpp"
#include <iostream>

int main(int argn, char** argv) {
    using namespace ce;
    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simple ray-casting Iniciado");

        g_registry.createEntity("chimera_engine", "chimera_engine");

        g_service_locator.registerService(std::make_shared<Keyboard>());
        g_service_locator.registerService(std::make_shared<CanvasFB>("BSP Tree", 800, 600, false));
        // g_service_locator.registerService(std::make_shared<ViewProjection>()); // not used but necessary

        // Engine
        Engine engine;

        ColladaDom dom = loadFileCollada("./samples/raycasting/level.xml");
        colladaRegistryLoad(dom);

        Game* game = new Game();

        engine.getStack().pushState(game);
        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;
        SDL_Log("raycasting finalizado com sucesso");
        return 0;

    } catch (const std::string& ex) { SDL_Log("Falha grave: %s", ex.c_str()); } catch (...) {
        SDL_Log("Falha Desconhecida");
    }

    return -1;
}
