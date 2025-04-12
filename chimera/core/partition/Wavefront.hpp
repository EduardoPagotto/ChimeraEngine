#pragma once
#include "chimera/core/ServiceLocator.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/Mesh.hpp"

namespace ce {
void wavefrontObjLoad(const std::string& path, Mesh* mesh, std::string& fileMath);
void wavefrontMtlLoad(const std::string& path, Material* material);
} // namespace ce