#ifndef __CHIMERA_LOAD_OBJ__HPP
#define __CHIMERA_LOAD_OBJ__HPP

#include "Triangle.hpp"
#include "Vertex.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

void loadObj(const char* _fineName, MeshData* _mesh);
void convertMeshDataTriangle(MeshData* _pMesh, std::vector<Triangle>* _pListPolygon);
void convertMeshDataVertexData(MeshData* _pMesh, std::vector<VertexData>& outData);
void convertMeshDataVertexDataFull(MeshData* _pMesh, std::vector<VertexDataFull>& outData);

} // namespace Chimera

#endif