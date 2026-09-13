#pragma once
#include "ColladaDom.hpp"
#include <entt/entt.hpp>

namespace ce {
    void colladaRenderLoad(std::shared_ptr<entt::registry> registry, ColladaDom& dom);
}
