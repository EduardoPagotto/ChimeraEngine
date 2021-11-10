#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

class LibraryMaterials : public Library {

  public:
    LibraryMaterials(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity);
    virtual ~LibraryMaterials();
    Material* target();

  private:
    Entity entity;
};
} // namespace Chimera