#pragma once

#include "cevk/VulkanTexture.hpp"
#include <entt/entt.hpp>
#include <optional>

struct MaterialData {
    std::string name;
    float metallic;
    float roughness;
    glm::vec4 baseColorFactor;
    glm::vec3 emissiveFactor;
    std::optional<entt::id_type> metallicRoughnessTexture;
    std::optional<entt::id_type> baseColorTexture;
};

struct MaterialLoader {
    using result_type = MaterialData;
    MaterialData operator()(const std::string& filePath) const { return MaterialData{}; }
};

namespace ce {
    struct AssetManager {
        entt::resource_cache<VulkanTexture, TextureLoader> texture;
    };
} // namespace ce
