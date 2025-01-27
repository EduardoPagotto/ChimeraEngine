#pragma once
#include "Collada.hpp"

namespace Chimera {
ColladaDom loadFileCollada(const std::string& file);
void colladaRegistryLoad(ColladaDom& dom, std::shared_ptr<ServiceLocator> serviceLoc);
} // namespace Chimera