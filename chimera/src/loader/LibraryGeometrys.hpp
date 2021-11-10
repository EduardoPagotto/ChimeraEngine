#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/node/NodeMesh.hpp"

namespace Chimera {

class LibraryGeometrys : public Library {

  public:
    LibraryGeometrys(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryGeometrys();
    NodeMesh* target();

  private:
    int getSource(tinyxml2::XMLElement* _source, std::vector<float>& _arrayValores);
    std::string loadMeshCollada(tinyxml2::XMLElement* _nNode, NodeMesh* _pMesh);
};
} // namespace Chimera