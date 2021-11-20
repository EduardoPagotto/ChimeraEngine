#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

class LibraryCameras : public Library {

  public:
    LibraryCameras(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity) : Library(_root, _url), entity(entity) {}
    virtual ~LibraryCameras() {}
    void target();

  private:
    void loadbase(tinyxml2::XMLElement* _nNode, ICamera3D* cam);
    Entity entity;
};
} // namespace Chimera
