#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
ColladaDom loadFileCollada(const std::string& file);
void colladaLoad(Registry& r, const std::string& pathFile);
} // namespace Chimera