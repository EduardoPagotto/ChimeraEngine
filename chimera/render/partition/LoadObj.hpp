#pragma once
#include "chimera/core/visible/Material.hpp" //FIXME: Mover para loader!!!!!
#include "chimera/core/visible/Mesh.hpp"

namespace Chimera {
int loadObjFile(const std::string& pathFile, Mesh* mesh, Material* material);
} // namespace Chimera
