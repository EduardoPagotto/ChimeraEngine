#include "Game.hpp"
#include "chimera/base/Engine.hpp"
#include "chimera/base/Mouse.hpp"
#include "chimera/collada/colladaLoad.hpp"
#include "chimera/core/gl/CanvasGL.hpp"
#include "chimera/core/gl/FontMng.hpp"
#include "chimera/core/gl/ShaderMng.hpp"
#include <config_params.hpp>
#include <iostream>

int main(int argn, char** argv) {

    using namespace ce;
    try {
        SDL_SetAppMetadata(std::string(project_name).c_str(), std::string(project_version).c_str(),
                           "com.mechanical.engine");

        SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Models3 Iniciado");

        // Registry to entt
        g_service_locator.registerService(std::make_shared<CanvasGL>("Simples", 800, 600, false));
        g_service_locator.registerService(std::make_shared<Mouse>());
        g_service_locator.registerService(std::make_shared<ViewProjection>(0.0f)); // View projection
        g_service_locator.registerService(std::make_shared<ShaderMng>());
        g_service_locator.registerService(std::make_shared<FontMng>());
        g_service_locator.registerService(std::make_shared<TextureMng>());

        Engine engine;

        ColladaDom dom = loadFileCollada("./samples/helloworld/level.xml");
        colladaRegistryLoad(dom);

        Game game(&engine);

        engine.getStack().pushState(&game);

        Collada::destroy(); // clean loader

        engine.run();
        SDL_Log("Loop de Game encerrado!!!!");
        SDL_Log("AppShader finalizado com sucesso");
        return 0;

    } catch (const std::exception& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.what()); // Exception generica
    } catch (const std::string& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.c_str()); // Exception string
    } catch (...) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro Desconhecida"); // Exception geral
    }

    return -1;
}
