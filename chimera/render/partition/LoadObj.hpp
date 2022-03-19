#pragma once
#include "chimera/core/visible/Material.hpp"
#include "chimera/render/VertexData.hpp"

namespace Chimera {
int loadObjFile(const std::string& pathFile, Mesh* mesh, Material* material);
} // namespace Chimera
