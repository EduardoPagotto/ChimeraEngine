#ifndef __MESH_UTIL_H__
#define __MESH_UTIL_H__

#include "Material.h"
#include "Mesh.h"

namespace Chimera {

void debugDados(Mesh *pMesh);
Mesh* createMeshParallelepiped(Node *_pParent, const std::string &_name, const glm::vec3 &_size, Material *_pMaterial); 
Mesh* createMeshParallelepiped2(Node *_pParent, const std::string &_name, const glm::vec3 &_size, Material *_pMaterial); 
   
}

#endif

