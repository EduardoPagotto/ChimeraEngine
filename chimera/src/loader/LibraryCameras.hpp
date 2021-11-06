#ifndef __CHIMERA_LOADER_LIBRARY_CAMERAS__HPP
#define __CHIMERA_LOADER_LIBRARY_CAMERAS__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/node/NodeCamera.hpp"

namespace Chimera {

class LibraryCameras : public Library {

  public:
    LibraryCameras(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryCameras();
    Chimera::NodeCamera* target();

  private:
    void loadbase(tinyxml2::XMLElement* _nNode, Chimera::NodeCamera* _pCamera);
    void extra(tinyxml2::XMLElement* _nNode, Chimera::NodeCamera* _pCamera);
};
} // namespace Chimera

#endif