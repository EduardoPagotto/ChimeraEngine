#include "LibraryMaterials.hpp"
#include "LibraryEffects.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

LibraryMaterials::LibraryMaterials(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity) : Library(_root, _url) {
    this->entity = entity;
}

LibraryMaterials::~LibraryMaterials() {}

Material* LibraryMaterials::target() {

    tinyxml2::XMLElement* l_nMat = root->FirstChildElement("library_materials")->FirstChildElement("material");
    for (l_nMat; l_nMat; l_nMat = l_nMat->NextSiblingElement()) {

        std::string l_id = l_nMat->Attribute("id");
        if (url.compare(l_id) == 0) {
            std::string url = l_nMat->FirstChildElement("instance_effect")->Attribute("url");
            LibraryEffects le(root, url, entity);
            Material* retorno = le.target();
            return retorno;
        }
    }
    throw Exception("Material nao encontrado Node: " + url);
}

} // namespace Chimera