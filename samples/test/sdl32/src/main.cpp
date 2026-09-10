// #include "AssetManager.hpp"
#include "Game.hpp"
#include "cevk_engine/ScreenVK.hpp"
#include "cevk_infra/Engine.hpp"
#include "cevk_infra/InputManager.hpp"
#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <memory>
// #include "cevk/VulkanTexture.hpp"
//  #include "cevk/TextureBindless.hpp"
//  #include "cevk_engine/AssetManager.hpp"
//  #include "cevk_infra/ServiceLocator.hpp"
//  #include "AssetManager.hpp"

int main(int argc, char* argv[]) {

    using namespace ce;

    auto result = EXIT_SUCCESS;

    // Habilita todas as mensagens em modo Debug
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG);

    try {
        std::shared_ptr<entt::registry> registry = std::make_shared<entt::registry>();

        using enum InputEnable;
        InputEnable in = Mouse | Keyboard;

        registry->ctx().emplace<std::shared_ptr<InputManager>>(std::make_shared<InputManager>(in, true));
        auto& ctx = registry->ctx().emplace<std::shared_ptr<VulkanContext>>(std::make_shared<VulkanContext>());

        ctx->createWindow("Teste SDL32");

        // auto& assetManager = registry->ctx().emplace<AssetManager>();
        // entt::id_type id_textura = entt::hashed_string{"id_tex"};
        // assetManager.texture.load(id_textura, ctx, "assets/player.png");
        // auto& im = registry.ctx().get<InputManager>();

        std::shared_ptr<ce::ScreenVK> scr = std::make_shared<ScreenVK>(ctx);

        Engine engine(registry, scr);
        std::shared_ptr<IStateMachine> game = std::make_shared<Game>(registry, scr);

        // teste(ctx);

        engine.getStack().pushState(game);
        engine.run();

        // g_loc.destroy();

        SDL_Log("Finalizado");

    } catch (const std::runtime_error& e) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        result = EXIT_FAILURE;

    } catch (...) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
        result = EXIT_FAILURE;
    }

    return result;
}
