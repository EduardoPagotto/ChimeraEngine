#ifndef __CHIMERA_LOADER_LIBRARY_CAMERAS__HPP
#define __CHIMERA_LOADER_LIBRARY_CAMERAS__HPP

#include "Camera.h"
#include "Library.h"

namespace ChimeraLoaders {

class LibraryCameras : public Library {

  public:
    LibraryCameras(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryCameras();
    Chimera::Camera* target();

  private:
    void loadbase(tinyxml2::XMLElement* _nNode, Chimera::Camera* _pCamera);
    void extra(tinyxml2::XMLElement* _nNode, Chimera::Camera* _pCamera);
};
} // namespace ChimeraLoaders

#endif