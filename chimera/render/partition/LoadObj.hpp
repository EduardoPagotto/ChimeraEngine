#pragma once

#include "chimera/core/VertexData.hpp"
#include "chimera/render/Material.hpp"

namespace Chimera {
int loadObjFile(const std::string& pathFile, MeshData* mesh, Material* material);
} // namespace Chimera
