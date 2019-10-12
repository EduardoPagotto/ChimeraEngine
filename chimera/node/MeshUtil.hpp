#ifndef __CHIMERA_MESH_UTIL__HPP
#define __CHIMERA_MESH_UTIL__HPP

#include "Material.hpp"
#include "Mesh.hpp"

namespace Chimera {

void debugDados(Mesh* pMesh);
Mesh* createMeshParallelepiped(Node* _pParent, const std::string& _name, const glm::vec3& _size, Material* _pMaterial);
Mesh* createMeshParallelepiped2(Node* _pParent, const std::string& _name, const glm::vec3& _size, Material* _pMaterial);

Mesh* createEmpty(Node* _pParent, const std::string& _name, Material* _pMaterial);

} // namespace Chimera

#endif
