#ifndef __CHIMERA_LOADER_LIBRARY_LIGHTS__HPP
#define __CHIMERA_LOADER_LIBRARY_LIGHTS__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/node/Light.hpp"

namespace ChimeraLoaders {

class LibraryLights : public Library {

  public:
    LibraryLights(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryLights();
    Chimera::Light* target();

  private:
    glm::vec4 getColor(tinyxml2::XMLElement* l_nColorVal);
    std::tuple<glm::vec4, Chimera::LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);
};
} // namespace ChimeraLoaders

#endif