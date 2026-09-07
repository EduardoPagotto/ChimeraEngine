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

void teste(std::shared_ptr<ce::VulkanContext> ctx) {

    // using namespace ce;

    // auto assetManager = g_loc.getService<AssetManager>();
    // TextureBindless texBindless(ctx);

    // auto logo = assetManager->load<VulkanTexture>("logo_jogos", "./assets/textures/grid1.png");

    // texBindless.allocTexture(logo);

    // {
    //     // 3. Entra em uma nova fase (Cria novo escopo)
    //     assetManager->pushScope();

    //     // Carrega textura específica desta fase
    //     auto bg = assetManager->load<VulkanTexture>("background", "./assets/textures/grid2.png");

    //     texBindless.allocTexture(bg);

    //     // Se pedir a "logo_jogos" aqui, ele busca e encontra no escopo Global pai
    //     auto logoMenu = assetManager->load<VulkanTexture>("logo_jogos", "./assets/textures/grid3.png");

    // } // 5. Sai da fase (PopScope deve ser chamado manualmente ou via RAII)
    // assetManager->popScope();
    // // Neste ponto, "fase1_bg.png" foi destruído do Vulkan automaticamente.
    // // "logo.png" continua vivo no escopo global.
}

int main(int argc, char* argv[]) {

    using namespace ce;

    auto result = EXIT_SUCCESS;

    // Habilita todas as mensagens em modo Debug
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG);

    try {

        std::shared_ptr<VulkanContext> ctx = std::make_shared<VulkanContext>();
        ctx->createWindow("Teste z1");

        // auto& assetManager = registry.ctx().emplace<AssetManager>();
        // entt::id_type id_textura = entt::hashed_string{"id_tex"};
        // assetManager.texture.load(id_textura, ctx, "assets/player.png");
        using enum InputEnable;

        entt::registry registry;

        InputEnable in = Mouse | Keyboard;
        registry.ctx().emplace<InputManager>(in, true);
        // auto& im = registry.ctx().get<InputManager>();

        std::shared_ptr<ce::ScreenVK> scr = std::make_shared<ScreenVK>(ctx);

        Engine engine(registry, scr);
        std::shared_ptr<IStateMachine> game = std::make_shared<Game>(ctx, scr);

        teste(ctx);

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
