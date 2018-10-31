#ifndef __CHIMERA_LOADER_LIBRARY_CAMERA_H
#define __CHIMERA_LOADER_LIBRARY_CAMERA_H

#include "Library.h"
#include "Camera.h"

namespace ChimeraLoaders {

class LibraryCameras: public Library {

public:
    LibraryCameras(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryCameras();
    Chimera::Camera *target();
private:
    void loadbase ( tinyxml2::XMLElement* _nNode, Chimera::Camera *_pCamera);
    void extra(tinyxml2::XMLElement* _nNode, Chimera::Camera *_pCamera);
    //std::tuple<Chimera::Color, Chimera::LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);
};
}

#endif