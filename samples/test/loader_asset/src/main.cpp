#include "cevk_infra/AssetManager.hpp"
#include "cevk_infra/Loader.hpp"

int main() {

    using namespace ce;

    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);

    std::shared_ptr<VulkanContext> ctx = std::make_shared<VulkanContext>();
    ctx->createWindow("Teste z1");

    entt::registry registry;
    auto& assetManager = registry.ctx().emplace<AssetManager>();

    Loader loader("./assets/seahawk/Seahawk.gltf", &registry);
    loader.getImages("./assets/seahawk/", ctx);

    for (auto&& [id, handle] : assetManager.texture) {
        const VulkanTexture tex = *handle;
        SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "index: %d", tex.getIndex());
    }

    assetManager.texture.clear();

    return 0;
}
