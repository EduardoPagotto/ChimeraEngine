#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/render/Material.hpp"

namespace Chimera {

class LibraryEffects : public Library {

  public:
    LibraryEffects(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryEffects();
    Material* target();

  private:
    void loadNewParam(tinyxml2::XMLElement* _nProfile);
    glm::vec4 getColor(tinyxml2::XMLElement* l_nColorVal);
    void loadColors(tinyxml2::XMLElement* _nProfile, Material* _pMat);
    Texture* getTexture(tinyxml2::XMLElement* _nTex);

    std::map<std::string, std::string> mapaEfeito;
};
} // namespace Chimera
