#ifndef __CHIMERA_LOADER_LIBRARY_EFFECTS__HPP
#define __CHIMERA_LOADER_LIBRARY_EFFECTS__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/render/Material.hpp"

namespace ChimeraLoaders {

class LibraryEffects : public Library {

  public:
    LibraryEffects(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryEffects();
    Chimera::Material* target();

  private:
    void loadNewParam(tinyxml2::XMLElement* _nProfile);
    glm::vec4 getColor(tinyxml2::XMLElement* l_nColorVal);
    void loadColors(tinyxml2::XMLElement* _nProfile, Chimera::Material* _pMat);
    Chimera::TextureImg* getTexture(tinyxml2::XMLElement* _nTex);

    std::map<std::string, std::string> mapaEfeito;
};
} // namespace ChimeraLoaders
#endif
