#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/render/Material.hpp"

namespace Chimera {

class LibraryMaterials : public Library {

  public:
    LibraryMaterials(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryMaterials();
    Material* target();

  private:
};
} // namespace Chimera