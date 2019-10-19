#include "LibraryMaterials.hpp"
#include "LibraryEffects.hpp"
#include "chimera/core/Exception.hpp"

namespace ChimeraLoaders {

LibraryMaterials::LibraryMaterials(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}

LibraryMaterials::~LibraryMaterials() {}

Chimera::MatData* LibraryMaterials::target() {

    tinyxml2::XMLElement* l_nMat = root->FirstChildElement("library_materials")->FirstChildElement("material");
    for (l_nMat; l_nMat; l_nMat = l_nMat->NextSiblingElement()) {

        std::string l_id = l_nMat->Attribute("id");
        if (url.compare(l_id) == 0) {
            std::string url = l_nMat->FirstChildElement("instance_effect")->Attribute("url");
            LibraryEffects le(root, url);
            Chimera::MatData* retorno = le.target();
            return retorno;
        }
    }
    throw Chimera::Exception("Material nao encontrado Node: " + url);
}

} // namespace ChimeraLoaders