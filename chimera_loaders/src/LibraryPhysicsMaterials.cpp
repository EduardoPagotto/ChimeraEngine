#include "LibraryPhysicsMaterials.h"
#include "ExceptionChimera.h"
#include "LoaderDaeUtils.h"

namespace ChimeraLoaders {

LibraryPhysicsMaterials::LibraryPhysicsMaterials(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryPhysicsMaterials::~LibraryPhysicsMaterials() {
}

Chimera::PhysicMaterial *LibraryPhysicsMaterials::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_physics_materials");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("physics_material");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    tinyxml2::XMLElement* l_nTec = l_nNodeBase->FirstChildElement ( "technique_common" );
                    if ( l_nTec != nullptr ) {

                        Chimera::PhysicMaterial *pMaterial = new Chimera::PhysicMaterial(l_id);

                        const char *val = Chimera::getUniqueTextElemetName(l_nTec, "dynamic_friction");
                        if (val != nullptr)
                            pMaterial->setFrictionDynamic ( atof ( val ) );
                        
                        val = Chimera::getUniqueTextElemetName(l_nTec, "restitution");
                        if (val != nullptr)
                            pMaterial->setRestitution ( atof ( val ) );

                        return pMaterial;
                    }

                    throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "technique_common, nao encontrado physics_material");        
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "physics_material" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_physics_materials" ) );
    }
}
}