#pragma once
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/loader/Library.hpp"

namespace Chimera {

class LibraryMaterials : public Library {

  public:
    LibraryMaterials(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity) : Library(_root, _url), entity(entity) {}
    virtual ~LibraryMaterials() {}
    void target();

  private:
    Entity entity;
};
} // namespace Chimera