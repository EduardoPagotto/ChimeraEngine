#ifndef __CHIMERA_LOADER_LIBRARY_EFFECTS_H
#define __CHIMERA_LOADER_LIBRARY_EFFECTS_H

#include "Library.h"
#include "Material.h"
#include "TextureManager.h"

namespace ChimeraLoaders {

class LibraryEffects: public Library {

public:
    LibraryEffects(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryEffects();
    Chimera::Material *target();
private:
    void loadNewParam(tinyxml2::XMLElement* _nProfile);
    Chimera::Color getColor( tinyxml2::XMLElement* l_nColorVal);
    void loadColors(tinyxml2::XMLElement* _nProfile, Chimera::Material *_pMat);
    unsigned getTexture(tinyxml2::XMLElement* _nTex);

    Chimera::TextureManager *pTexManager;
    std::map<std::string, std::string> mapaEfeito;
};
}
#endif
