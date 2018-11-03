#include "LibraryLights.h"
#include "ExceptionChimera.h"
#include "LoaderDaeUtils.h"

namespace ChimeraLoaders {

LibraryLights::LibraryLights(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryLights::~LibraryLights() {
}

Chimera::Light *LibraryLights::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_lights");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("light");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    Chimera::Light *pLight = new Chimera::Light (nullptr,l_id );

                    auto ret_data = loadDiffuseLightColor(l_nNodeBase);
                    pLight->setDiffuse(std::get<0>(ret_data));
                    pLight->setType(std::get<1>(ret_data));

                    return pLight;      
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "light" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_lights" ) );
    }
}

std::tuple<Chimera::Color, Chimera::LightType> LibraryLights::loadDiffuseLightColor(tinyxml2::XMLElement* _nNode)
{
    tinyxml2::XMLElement *l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "point" );
    if ( l_nPoint != nullptr ) {

        std::vector<float> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement("color")->GetText();
        Chimera::loadArrayBtScalar ( l_val, l_arrayF );
        Chimera::Color cor(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);

        return std::make_tuple(cor, Chimera::LightType::POSITIONAL);
    }

    l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "directional" );
    if ( l_nPoint != nullptr ) {

        std::vector<float> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement ( "color" )->GetText();
        Chimera::loadArrayBtScalar ( l_val, l_arrayF );

        Chimera::Color cor(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);

        return std::make_tuple(cor, Chimera::LightType::DIRECTIONAL);
    }

}

}