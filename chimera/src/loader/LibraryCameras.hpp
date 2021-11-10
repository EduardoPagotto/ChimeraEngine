#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

class LibraryCameras : public Library {

  public:
    LibraryCameras(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity);
    virtual ~LibraryCameras();
    NodeCamera* target();

  private:
    void loadbase(tinyxml2::XMLElement* _nNode, ICamera3D* cam);
    // void extra(tinyxml2::XMLElement* _nNode, NodeCamera* _pCamera);

    Entity entity;
};
} // namespace Chimera
