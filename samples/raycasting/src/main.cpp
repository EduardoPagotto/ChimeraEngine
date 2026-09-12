#include "Game.hpp"
#include "chimera_base/Engine.hpp"
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

        SDL_Log("Simple ray-casting Iniciado");

        std::shared_ptr<entt::registry> registry = std::make_shared<entt::registry>();

        using enum ce::InputEnable;
        InputEnable in = Mouse | Keyboard;

        registry->ctx().emplace<std::shared_ptr<InputManager>>(std::make_shared<InputManager>(in, true));

        std::shared_ptr<CanvaFB> canva = std::make_shared<CanvaFB>("Teste Framebuffer SDL3", 800, 600, false);

        // // Engine
        Engine engine(registry, canva);

        std::shared_ptr<IStateMachine> game = std::make_shared<Game>(registry, canva);

        engine.getStack().pushState(game);
        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");

        return 0;

    } catch (const std::runtime_error& e) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        result = EXIT_FAILURE;

    } catch (...) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
        result = EXIT_FAILURE;
    }

    return result;
}
