#pragma once
#include "chimera/core/Registry.hpp"
#include "chimera/loader/Library.hpp"
#include "chimera/render/bullet/Solid.hpp"

namespace Chimera {

class LibraryPhysicModels : public Library {

  public:
    LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}
    virtual ~LibraryPhysicModels() {}
    void target(std::map<std::string, Solid*>& _mapSolids);
    void target2(const std::string& body, const std::string target, Registry* reg);

  private:
    void loadColladaShape(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, Solid* _pPhysic);
};
} // namespace Chimera
