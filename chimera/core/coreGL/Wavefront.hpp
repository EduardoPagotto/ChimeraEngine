#pragma once
#include "Material.hpp"
#include "chimera/core/space/Mesh.hpp"

namespace ce {
void wavefrontObjLoad(const std::string& path, Mesh* mesh, std::string& fileMath);
void wavefrontMtlLoad(const std::string& path, Material* material);
} // namespace ce
