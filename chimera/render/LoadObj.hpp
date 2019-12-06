#ifndef __CHIMERA_LOAD_OBJ__HPP
#define __CHIMERA_LOAD_OBJ__HPP

#include "chimera/render/Material.hpp"
#include "chimera/render/MeshData.hpp"

namespace Chimera {

void loadObjMtl(const std::string& _fineNameMtl, MeshData& _mesh, Material& _pMaterial);
void loadMtl(const std::string& _fineNameMtl, Material* _pMaterial);
void loadObj(const std::string& _fineNameObj, MeshData& _mesh, std::string& _materialFile);
void loadTerrain(const std::string& _fileName, MeshData& _mesh);

} // namespace Chimera

#endif