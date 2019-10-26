#ifndef __CHIMERA_LOAD_OBJ__HPP
#define __CHIMERA_LOAD_OBJ__HPP

#include "Material.hpp"
#include "MeshData.hpp"
#include "Triangle.hpp"
#include "Vertex.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

void loadObjMtl(const std::string& _fineNameMtl, MeshData& _mesh, Material& _pMaterial);

void loadMtl(const std::string& _fineNameMtl, Material* _pMaterial);
void loadObj(const std::string& _fineNameObj, MeshData& _mesh, std::string& _materialFile);
void convertMeshDataTriangle(MeshData* _pMesh, std::vector<Triangle>& vecTriangle);
void convertMeshDataVertexData(MeshData* _pMesh, std::vector<VertexData>& outData);

} // namespace Chimera

#endif