#include "LibraryPhysicsMaterials.hpp"

namespace Chimera {

PhysicMaterial* LibraryPhysicsMaterials::target() {

    for (tinyxml2::XMLElement* l_nPyMat = root->FirstChildElement("library_physics_materials")->FirstChildElement("physics_material");
         l_nPyMat; l_nPyMat = l_nPyMat->NextSiblingElement()) {

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
    throw std::string("Physics material nao encontrado: " + url);
}
} // namespace Chimera