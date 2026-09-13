#pragma once
#include "ColladaDom.hpp"

namespace ce {
    ColladaDom loadFileCollada(const std::string& file);
    void colladaRegistryLoad(ColladaDom& dom);
} // namespace ce
