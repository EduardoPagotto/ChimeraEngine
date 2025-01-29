#pragma once
#include "chimera/core/ServiceLocator.hpp"
#include "chimera/core/collada/ColladaDom.hpp"

namespace ce {
void colladaRenderLoad(ColladaDom& dom, std::shared_ptr<ServiceLocator> serviceLoc);
}
