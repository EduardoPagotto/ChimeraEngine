#include "Game.hpp"
#include "chimera_base/Engine.hpp"
#include "chimera_base/aux/ViewProjection.hpp"
#include "chimera_collada/colladaLoad.hpp"
#include "chimera_core/gl/CanvasGL.hpp"
#include "chimera_core/gl/ShaderMng.hpp"
#include "chimera_core/gl/TextureMng.hpp"
#include "chimera_render/scene/Scene.hpp"
#include <config_params.hpp>
#include <memory>

int main(int argn, char** argv) {

    auto result = EXIT_SUCCESS;

    using namespace ce;

    try {
        SDL_SetAppMetadata(std::string(project_name).c_str(), std::string(project_version).c_str(),
                           "com.mechanical.engine");

        // Habilita todas as mensagens em modo Debug
        SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);

        SDL_Log("Simnples Iniciado");

        // Registry to entt
        std::shared_ptr<entt::registry> registry = std::make_shared<entt::registry>();

        // using enum ce::InputEnable;
        // InputEnable in = Mouse | Keyboard;

        registry->ctx().emplace<std::shared_ptr<InputManager>>(std::make_shared<InputManager>());
        registry->ctx().emplace<std::shared_ptr<CanvasGL>>(std::make_shared<CanvasGL>("BSP Tree", 1800, 600, false));
        registry->ctx().emplace<std::shared_ptr<ViewProjection>>(std::make_shared<ViewProjection>(0.5F));
        registry->ctx().emplace<std::shared_ptr<ShaderMng>>(std::make_shared<ShaderMng>());
        registry->ctx().emplace<std::shared_ptr<TextureMng>>(std::make_shared<TextureMng>());

        // Engine
        std::shared_ptr<CanvasGL> canva = registry->ctx().get<std::shared_ptr<CanvasGL>>();
        Engine engine(registry, canva);

        ColladaDom dom = loadFileCollada("./samples/simples/level.xml");

        colladaRegistryLoad(registry, dom);

        std::shared_ptr<Scene> scene = std::make_shared<Scene>(registry);
        std::shared_ptr<IStateMachine> game = std::make_shared<Game>(registry);

        engine.getStack().pushState(scene);
        engine.getStack().pushState(game);

        engine.run();
        SDL_Log("Loop de Game encerrado!!!!");

    } catch (const std::runtime_error& e) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        result = EXIT_FAILURE;

    } catch (...) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
        result = EXIT_FAILURE;
    }

    return result;
}
