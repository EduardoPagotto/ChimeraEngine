#pragma once
#include "Collada.hpp"

namespace Chimera {
ColladaDom loadFileCollada(const std::string& file);
void colladaRegistryLoad(ColladaDom& dom);
} // namespace Chimera