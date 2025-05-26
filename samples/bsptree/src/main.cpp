#include "Game.hpp"
#include "chimera/base/Engine.hpp"
#include "chimera/collada/colladaLoad.hpp"
#include "chimera/core/gl/CanvasGL.hpp"
#include "chimera/core/gl/ShaderMng.hpp"
#include "chimera/render/scene/Scene.hpp"
#include <config_params.hpp>

int main(int argn, char** argv) {
    using namespace ce;
    try {
        SDL_SetAppMetadata(std::string(project_name).c_str(), std::string(project_version).c_str(),
                           "com.mechanical.engine");

        SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("BSPTree Iniciado");

        // Registry to entt
        auto reg = std::make_shared<Registry>();

        // Services shared inside all parts
        // Canvas, Mouse, keyboard, Joystick, gamepad, view's
        g_service_locator.registerService(std::make_shared<CanvasGL>("BSP Tree", 1800, 600, false));
        g_service_locator.registerService(std::make_shared<Mouse>());
        g_service_locator.registerService(std::make_shared<ViewProjection>(0.5f)); // View projection
        g_service_locator.registerService(std::make_shared<ShaderMng>());
        g_service_locator.registerService(std::make_shared<TextureMng>());
        // Engine
        Engine engine;

        ColladaDom dom = loadFileCollada("./samples/bsptree/bsp_level.xml");
        colladaRegistryLoad(dom);

        Scene scene;
        Game game;

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
