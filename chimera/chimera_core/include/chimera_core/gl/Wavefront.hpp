#pragma once
#include "chimera_core/gl/Material.hpp"
#include "chimera_core/visible/Mesh.hpp"

namespace ce {
    void wavefrontObjLoad(const std::string& path, Mesh* mesh, std::string& fileMath);
    void wavefrontMtlLoad(const std::string& path, std::shared_ptr<Material> material);
} // namespace ce
