#include "Game.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
//#include "chimera/render/collada/ColladaRender.hpp"
#include <iostream>

int main(int argn, char** argv) {
    using namespace Chimera;
    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Iniciado");

        Scene scene;

        ColladaDom dom = loadFileCollada("./samples/simples/level.xml");
        colladaRegistryLoad(dom, scene.getRegistry());
        // colladaRenderLoad(dom, scene.getRegistry());

        Engine engine(scene.getCanvas());

        Game* game = new Game(&scene, &engine);

        Collada::destroy(); // clean loader

        engine.run();

        delete game;

        SDL_Log("Sucesso");
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
