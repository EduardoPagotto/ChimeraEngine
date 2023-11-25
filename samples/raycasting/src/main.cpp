#include "Game.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/device/CanvasFB.hpp"
#include <iostream>

int main(int argn, char** argv) {
    using namespace Chimera;
    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simple ray-casting Iniciado");

        RegistryManager::create();

        Engine engine(new CanvasFB("BSP Tree", 800, 600, false), 0.0f);

        ColladaDom dom = loadFileCollada("./samples/raycasting/level.xml");
        colladaRegistryLoad(dom);

        engine.init();

        Game* game = new Game(&engine);

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
