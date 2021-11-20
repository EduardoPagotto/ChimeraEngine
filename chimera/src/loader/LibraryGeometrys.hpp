#pragma once
#include "chimera/core/Registry.hpp"
#include "chimera/loader/Library.hpp"
#include "chimera/render/VertexData.hpp"

namespace Chimera {

class LibraryGeometrys : public Library {

  public:
    LibraryGeometrys(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity) : Library(_root, _url), entity(entity) {}
    virtual ~LibraryGeometrys() {}
    void target();

  private:
    int getSource(tinyxml2::XMLElement* _source, std::vector<float>& _arrayValores);
    std::string loadMeshCollada(tinyxml2::XMLElement* _nNode, MeshData* meshData);

    Entity entity;
};
} // namespace Chimera