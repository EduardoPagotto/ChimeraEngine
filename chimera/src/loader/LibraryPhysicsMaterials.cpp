#include "LibraryPhysicsMaterials.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

LibraryPhysicsMaterials::LibraryPhysicsMaterials(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}

LibraryPhysicsMaterials::~LibraryPhysicsMaterials() {}

PhysicMaterial* LibraryPhysicsMaterials::target() {

    tinyxml2::XMLElement* l_nPyMat = root->FirstChildElement("library_physics_materials")->FirstChildElement("physics_material");
    for (l_nPyMat; l_nPyMat; l_nPyMat = l_nPyMat->NextSiblingElement()) {

        std::string l_id = l_nPyMat->Attribute("id");
        if (url.compare(l_id) == 0) {
            tinyxml2::XMLElement* l_nTecDyn = l_nPyMat->FirstChildElement("technique_common")->FirstChildElement("dynamic_friction");
            tinyxml2::XMLElement* l_nTecRes = l_nPyMat->FirstChildElement("technique_common")->FirstChildElement("restitution");

            PhysicMaterial* pMaterial = new PhysicMaterial();
            if (l_nTecDyn)
                pMaterial->frictionDynamic = atof(l_nTecDyn->GetText());

            if (l_nTecRes)
                pMaterial->restitution = atof(l_nTecRes->GetText());

            return pMaterial;
        }
    }
    throw Exception("Physics material nao encontrado: " + std::string(url));
}
} // namespace Chimera