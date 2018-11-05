#include "LibraryImages.h"
//#include "Singleton.h"
#include "ExceptionChimera.h"

#include <tuple>

namespace ChimeraLoaders {

LibraryImages::LibraryImages(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
    //pTexManager = Chimera::Singleton<Chimera::TextureManager>::getRefSingleton();
}

LibraryImages::~LibraryImages() {
    //Chimera::Singleton<Chimera::TextureManager>::releaseRefSingleton();
}

std::tuple<std::string, std::string> LibraryImages::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_images");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("image");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    tinyxml2::XMLElement* l_nTec = l_nNodeBase->FirstChildElement ( "init_from" );
                    if ( l_nTec != nullptr ) {

                        std::string pathFileImage = l_nTec->GetText();
                        //return pathFileImage;
                        //pTexManager->fromFile(l_id, Chimera::TEX_SEQ::DIFFUSE, pathFileImage);
                        return std::make_tuple(l_id, pathFileImage);

                    }
                    throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "init_from, nao encontrado image");        
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "image" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_images" ) );
    }
}
}