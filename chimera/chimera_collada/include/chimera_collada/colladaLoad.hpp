#pragma once
#include "ColladaDom.hpp"
#include <entt/entt.hpp>

namespace ce {
    ColladaDom loadFileCollada(const std::string& file);
    void colladaRegistryLoad(std::shared_ptr<entt::registry> registry, ColladaDom& dom);
} // namespace ce
