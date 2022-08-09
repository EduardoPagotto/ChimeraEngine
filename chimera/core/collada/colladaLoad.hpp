#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
ColladaDom loadFileCollada(const std::string& file);
void colladaRegistryLoad(ColladaDom& dom, Registry& r);
} // namespace Chimera