#pragma once

#include "cevk/VulkanTexture.hpp"
#include <entt/entt.hpp>

namespace ce {
    struct AssetManager {
        entt::resource_cache<VulkanTexture, TextureLoader> texture;
    };
} // namespace ce
