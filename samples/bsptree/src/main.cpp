#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/visible/ShaderMng.hpp"
#include "chimera/render/scene/Scene.hpp"

int main(int argn, char** argv) {
    using namespace ce;
    try {
        // SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("BSPTree Iniciado");

        // Registry to entt
        auto reg = std::make_shared<Registry>();

        // Services shared inside all parts
        // Canvas, Mouse, keyboard, Joystick, gamepad, view's
        auto sl = std::make_shared<ServiceLocator>();
        sl->registerService(reg);
        sl->registerService(std::make_shared<CanvasGL>("BSP Tree", 1800, 600, false));
        sl->registerService(std::make_shared<Mouse>());
        sl->registerService(std::make_shared<ViewProjection>(0.5f)); // View projection
        sl->registerService(std::make_shared<ShaderMng>());
        sl->registerService(std::make_shared<TextureMng>());
        // Engine
        Engine engine(sl);

        ColladaDom dom = loadFileCollada("./samples/bsptree/bsp_level.xml");
        colladaRegistryLoad(dom, sl);

        Scene scene(sl);
        Game game(sl);

        engine.getStack().pushState(&scene);
        engine.getStack().pushState(&game);

        Collada::destroy(); // clean loader

        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
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
