#pragma once
#include "chimera/core/space/AABB.hpp"

namespace Chimera {
void spaceRenderAABB(const AABB& aabb);
void spaceRenderFrustum(const Frustum& frustum);
} // namespace Chimera