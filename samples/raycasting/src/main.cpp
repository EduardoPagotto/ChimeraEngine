#include "Game.hpp"
#include "chimera_base/Engine.hpp"
#include "chimera_base/ICanva.hpp"
#include "chimera_base/InputManager.hpp"
#include <stdexcept>

int main(int argn, char** argv) {

    auto result = EXIT_SUCCESS;

    using namespace ce;

    try {
        // SDL_SetAppMetadata(std::string(project_name).c_str(), std::string(project_version).c_str(),
        //                    "com.mechanical.engine");

        // Habilita todas as mensagens em modo Debug
        SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);

        SDL_Log("Ray-casting Iniciado");

        std::shared_ptr<entt::registry> registry = std::make_shared<entt::registry>();
        registry->ctx().emplace<std::shared_ptr<ICanva>>(std::make_shared<CanvaFB>("Ray-casting", 800, 600, false));
        registry->ctx().emplace<std::shared_ptr<InputManager>>(std::make_shared<InputManager>());

        // // Engine
        Engine engine(registry);

        std::shared_ptr<IStateMachine> game = std::make_shared<Game>(registry);

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
