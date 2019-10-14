#ifndef __CHIMERA_LOAD_OBJ__HPP
#define __CHIMERA_LOAD_OBJ__HPP

#include "MeshData.hpp"
#include "Triangle.hpp"
#include "Vertex.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

void loadObj(const char* _fineName, MeshData* _mesh);
void convertMeshDataTriangle(MeshData* _pMesh, std::vector<Triangle>& vecTriangle);
void convertTriangleVertexDataFull(std::vector<Triangle>& vecTriangle, std::vector<VertexDataFull>& outData);
void convertMeshDataVertexData(MeshData* _pMesh, std::vector<VertexData>& outData);
void convertMeshDataVertexDataFull(MeshData* _pMesh, std::vector<VertexDataFull>& outData);

} // namespace Chimera

#endif