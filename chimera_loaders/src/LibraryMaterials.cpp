#include "LibraryMaterials.h"
#include "ExceptionChimera.h"
//#include "LoaderDaeUtils.h"
#include "LibraryEffects.h"


namespace ChimeraLoaders {

LibraryMaterials::LibraryMaterials(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryMaterials::~LibraryMaterials() {
}

Chimera::Material *LibraryMaterials::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_materials");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("material");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    tinyxml2::XMLElement* l_nEff = l_nNodeBase->FirstChildElement("instance_effect");
                    if (l_nEff != nullptr) {

                        std::string url = l_nEff->Attribute("url");
                        
                        LibraryEffects le(root, url);
                        Chimera::Material* retorno = le.target();
                        return retorno;
                    }
                    throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado instance_effect do material: " + std::string ( url ) );     
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "material" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_materials" ) );
    }
}

}