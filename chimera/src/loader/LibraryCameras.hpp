#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/node/NodeCamera.hpp"

namespace Chimera {

class LibraryCameras : public Library {

  public:
    LibraryCameras(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryCameras();
    NodeCamera* target();

  private:
    void loadbase(tinyxml2::XMLElement* _nNode, NodeCamera* _pCamera);
    void extra(tinyxml2::XMLElement* _nNode, NodeCamera* _pCamera);
};
} // namespace Chimera
