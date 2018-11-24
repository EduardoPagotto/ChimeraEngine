#include "LibraryMaterials.hpp"
#include "ExceptionChimera.hpp"
#include "LibraryEffects.hpp"

namespace ChimeraLoaders {

LibraryMaterials::LibraryMaterials(tinyxml2::XMLElement* _root, const std::string& _url)
    : Library(_root, _url) {}

LibraryMaterials::~LibraryMaterials() {}

Chimera::Material* LibraryMaterials::target() {

    tinyxml2::XMLElement* l_nMat =
        root->FirstChildElement("library_materials")->FirstChildElement("material");
    for (l_nMat; l_nMat; l_nMat = l_nMat->NextSiblingElement()) {

        std::string l_id = l_nMat->Attribute("id");
        if (url.compare(l_id) == 0) {
            std::string url =
                l_nMat->FirstChildElement("instance_effect")->Attribute("url");
            LibraryEffects le(root, url);
            Chimera::Material* retorno = le.target();
            return retorno;
        }
    }
    throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ,
                                    "Material nao encontrado Node: " + url);
}

} // namespace ChimeraLoaders