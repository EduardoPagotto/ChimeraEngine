#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/collada/colladaLoad.hpp"

int main(int argn, char** argv) {
    using namespace Chimera;
    try {
        // SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("BSPTree Iniciado");

        Engine engine;

        ColladaDom dom = loadFileCollada("./samples/bsptree/bsp_level.xml");
        colladaRegistryLoad(dom, engine.getRegistry());

        engine.init();

        Scene scene(engine.getRegistry());

        Game* game = new Game(scene);

        engine.getStack().pushState(&scene);
        engine.getStack().pushState(game);

        Collada::destroy(); // clean loader

        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;

        SDL_Log("BSPTree finalizado com sucesso");
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
