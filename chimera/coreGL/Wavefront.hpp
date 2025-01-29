#pragma once
#include "Material.hpp"
#include "core/ServiceLocator.hpp"
#include "space/Mesh.hpp"

namespace ce {
void wavefrontObjLoad(const std::string& path, Mesh* mesh, std::string& fileMath);
void wavefrontMtlLoad(const std::string& path, Material* material, std::shared_ptr<ServiceLocator> serviceLoc);
} // namespace ce
