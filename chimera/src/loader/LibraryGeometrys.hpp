#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

class LibraryGeometrys : public Library {

  public:
    LibraryGeometrys(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity);
    virtual ~LibraryGeometrys();
    NodeMesh* target();

  private:
    int getSource(tinyxml2::XMLElement* _source, std::vector<float>& _arrayValores);
    std::string loadMeshCollada(tinyxml2::XMLElement* _nNode, MeshData* meshData);

    Entity entity;
};
} // namespace Chimera