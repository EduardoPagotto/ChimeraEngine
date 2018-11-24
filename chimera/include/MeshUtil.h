#ifndef __CHIMERA_MESH_UTIL__HPP
#define __CHIMERA_MESH_UTIL__HPP

#include "Material.h"
#include "Mesh.h"

namespace Chimera {

void debugDados(Mesh* pMesh);
Mesh* createMeshParallelepiped(Node* _pParent, const std::string& _name,
                               const glm::vec3& _size, Material* _pMaterial);
Mesh* createMeshParallelepiped2(Node* _pParent, const std::string& _name,
                                const glm::vec3& _size, Material* _pMaterial);

} // namespace Chimera

#endif
