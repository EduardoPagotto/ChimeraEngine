#ifndef __CHIMERA_LOADER_LIBRARY_GEOMETRYS__HPP
#define __CHIMERA_LOADER_LIBRARY_GEOMETRYS__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/node/NodeMesh.hpp"

namespace ChimeraLoaders {

class LibraryGeometrys : public Library {

  public:
    LibraryGeometrys(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryGeometrys();
    Chimera::NodeMesh* target();

  private:
    int getSource(tinyxml2::XMLElement* _source, std::vector<float>& _arrayValores);
    std::string loadMeshCollada(tinyxml2::XMLElement* _nNode, Chimera::NodeMesh* _pMesh);
};
} // namespace ChimeraLoaders

#endif