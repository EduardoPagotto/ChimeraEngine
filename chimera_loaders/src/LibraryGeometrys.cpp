#include "LibraryGeometrys.h"
#include "ExceptionChimera.h"
//#include "LoaderDaeUtils.h"
//#include "LibraryEffects.h"


namespace ChimeraLoaders {

LibraryGeometrys::LibraryGeometrys(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryGeometrys::~LibraryGeometrys() {
}

// Chimera::Material *LibraryGeometrys::target() {

//     tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_materials");
//     if ( l_nLib != nullptr ) {

//         tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("material");
//         if ( l_nNodeBase != nullptr ) {

//             while ( l_nNodeBase != nullptr ) {
//                 std::string l_id = l_nNodeBase->Attribute ( "id" );
//                 if (url.compare(l_id) == 0) {

//                     tinyxml2::XMLElement* l_nEff = l_nNodeBase->FirstChildElement("instance_effect");
//                     if (l_nEff != nullptr) {

//                         std::string url = l_nEff->Attribute("url");
                        
//                         LibraryEffects le(root, url);
//                         Chimera::Material* retorno = le.target();
//                         return retorno;
//                     }
//                     throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado instance_effect do material: " + std::string ( url ) );     
//                 }
//                 l_nNodeBase = l_nNodeBase->NextSiblingElement();
//             }
//             throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
//         } else {
//             throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "material" ) );
//         }
//     } else {
//         throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_materials" ) );
//     }
// }

// std::tuple<Chimera::Color, Chimera::LightType> LibraryLights::loadDiffuseLightColor(tinyxml2::XMLElement* _nNode)
// {
//     tinyxml2::XMLElement *l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "point" );
//     if ( l_nPoint != nullptr ) {

//         std::vector<float> l_arrayF;
//         const char *l_val = l_nPoint->FirstChildElement("color")->GetText();
//         Chimera::loadArrayBtScalar ( l_val, l_arrayF );
//         Chimera::Color cor(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);

//         return std::make_tuple(cor, Chimera::LightType::POSITIONAL);
//     }

//     l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "directional" );
//     if ( l_nPoint != nullptr ) {

//         std::vector<float> l_arrayF;
//         const char *l_val = l_nPoint->FirstChildElement ( "color" )->GetText();
//         Chimera::loadArrayBtScalar ( l_val, l_arrayF );

//         Chimera::Color cor(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);

//         return std::make_tuple(cor, Chimera::LightType::DIRECTIONAL);
//     }

// }

}