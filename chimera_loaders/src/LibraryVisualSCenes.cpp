#include "LibraryVisualSCenes.h"
#include "ExceptionChimera.h"
#include "LoaderDaeUtils.h"

namespace ChimeraLoaders {

LibraryVisualScenes::LibraryVisualScenes(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryVisualScenes::~LibraryVisualScenes() {
}

Chimera::Group *LibraryVisualScenes::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_visual_scenes");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("visual_scene");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    Chimera::Group *pRootNode = new Chimera::Group(nullptr, l_id );

                    tinyxml2::XMLElement* l_nNode = l_nNodeBase->FirstChildElement ("node");

                    do {

                        const char * l_idR = l_nNode->Attribute ( "id" );
                        const char * l_name = l_nNode->Attribute ( "name" );
                        const char * l_type = l_nNode->Attribute ( "type" );

                        tinyxml2::XMLElement* l_nDadoNode = l_nNode->FirstChildElement();
                        if ( l_nDadoNode != nullptr ) {
                            carregaNode ( (Chimera::Node*)pRootNode, l_nDadoNode, l_idR, l_name, l_type );
                        } else {
                            //log->warn("Node sem filho");
                        }

                        l_nNode = l_nNode->NextSiblingElement();
                    } while ( l_nNode != nullptr );

                    return pRootNode;

                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "visual_scene" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_visual_scenes" ) );
    }
}


void LibraryVisualScenes::carregaNode ( Chimera::Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type ) {

    // glm::mat4 l_pTransform;
    // Node *pLastNodeDone = nullptr;

    // while ( _nNode != nullptr ) {

    //     tinyxml2::XMLElement* l_nNodeSourceData = nullptr;
    //     const char* l_url = _nNode->Attribute ( "url" );
    //     const char* l_target = _nNode->Attribute ( "target" );
    //     const char* l_nomeElemento = _nNode->Value();

    //     if ( strcmp ( l_nomeElemento, ( const char* ) "matrix" ) == 0 ) {

    //         const char* l_tipoTransform = _nNode->Attribute ( "sid" );
    //         if ( strcmp ( l_tipoTransform, ( const char* ) "transform" ) == 0 ) {

    //             l_pTransform = loadTransformMatrix ( _nNode->GetText() );

    //         } else {

    //             throw ExceptionChimera ( ExceptionCode::READ, "Matrix de transformacao invalida" );
    //             //TODO: implementar carga de posicao, rotacao e transformar em matricial em l_pTransform
    //         }


    //      } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_camera" ) == 0 ) {

    //        Camera *pCamera = LoaderDae::carregaCamera(root, _nNode, l_url, _id, _name);
    //        pCamera->setTransform ( l_pTransform );

    //        _pNodePai->addChild ( pCamera );

    //        pLastNodeDone = pCamera;

    //      } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_light" ) == 0 ) {

    //         loadNodeLib ( root, ( const char* ) &l_url[1], "library_lights", "light", &l_nNodeSourceData );

    //         Light *pLight = new Light (nullptr,_id );

    //         auto ret_data = LoaderDae::loadDiffuseLightColor(l_nNodeSourceData);
    //         pLight->setDiffuse(std::get<0>(ret_data));
    //         pLight->setType(std::get<1>(ret_data));
    //         pLight->setTransform ( l_pTransform );

    //         _pNodePai->addChild ( pLight );

    //         pLastNodeDone = pLight;

    //      } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_geometry" ) == 0 ) {

    //          const char* pUrl = _nNode->Attribute("url");
    //          std::string l_nomeGeo = &pUrl[1];

    //          Material *pMaterial = nullptr;
    //         // Texture *pTexture = nullptr;
	// 		 std::string textureName = "";

    //          Draw *pDraw = mapaGeometria[l_nomeGeo];

    //         tinyxml2::XMLElement* l_nBindMat = _nNode->FirstChildElement ( "bind_material" );
    //         if ( l_nBindMat != nullptr ) {
    //             tinyxml2::XMLElement* l_nTecnicCommon = l_nBindMat->FirstChildElement ( "technique_common" );
    //             if ( l_nTecnicCommon != nullptr ) {

    //                 tinyxml2::XMLElement* l_nInstanceMaterial = l_nTecnicCommon->FirstChildElement ( "instance_material" );

    //                 const char* pTarguet = l_nInstanceMaterial->Attribute("target");
    //                 loadNodeLib ( root, ( const char* ) &pTarguet[1], "library_materials", "material", &l_nNodeSourceData );
    //                 if (l_nNodeSourceData != nullptr) {

    //                     std::string nomeMaterial = retornaAtributo("id", l_nNodeSourceData);

    //                     pMaterial = new Material(nomeMaterial);

    //                     LoaderDae::loadMaterial(root, l_nNodeSourceData, pMaterial);

    //                     tinyxml2::XMLElement* l_nInstanceEffect = l_nNodeSourceData->FirstChildElement ( "instance_effect" );
    //                     if (l_nInstanceEffect != nullptr) {

    //                         tinyxml2::XMLElement* l_nNodeSourceTex = nullptr;
    //                         const char* l_pUrlEffect = l_nInstanceEffect->Attribute ( "url" );
    //                         loadNodeLib ( root, ( const char* ) &l_pUrlEffect[1], "library_effects", "effect", &l_nNodeSourceTex );

    //                         if (l_nNodeSourceTex != nullptr) {

    //                             tinyxml2::XMLElement* l_nProfile = l_nNodeSourceTex->FirstChildElement("profile_COMMON");
    //                             if (l_nProfile != nullptr) {

    //                                 tinyxml2::XMLElement* l_nParam = l_nProfile->FirstChildElement("newparam");
    //                                 if (l_nParam != nullptr) {
    //                                     const char* l_val = l_nParam->FirstChildElement("surface")->FirstChildElement("init_from")->GetText();
    //                                     if (l_val != nullptr)
	// 										textureName.assign(l_val);
    //                                 }
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }

	// 		if (pMaterial != nullptr) {

	// 			if (textureName.length() > 0) {
	// 				unsigned serialTex = texManager->getIdByName(textureName);
	// 				pMaterial->defineTextureByIndex(serialTex);
	// 			}

	// 			pDraw->setMaterial(pMaterial);
	// 		}

	// 		Solid *ph = mapaEntidadeFisica[_id];
    //         if (ph != nullptr) {

	// 			ph->setName(_id);//Preciso ???
	// 			ph->setMatrix(l_pTransform);
	// 			ph->addChild(pDraw);
	// 			pLastNodeDone = ph;
	// 			_pNodePai->addChild(ph);

    //         } else {

    //             Transform *pTrans = new Transform(_pNodePai, _id);
    //             pTrans->setMatrix(l_pTransform);
    //             pTrans->addChild(pDraw);
    //             pLastNodeDone = pTrans;
    //         }

    //      } else if ( strcmp ( l_nomeElemento, ( const char* ) "node" ) == 0 ) {

    //         const char * l_id = _nNode->Attribute ( "id" );
    //         const char * l_name = _nNode->Attribute ( "name" );
    //         const char * l_type = _nNode->Attribute ( "type" );

    //         if ( pLastNodeDone != nullptr ) {
    //             carregaNode ( pLastNodeDone, _nNode->FirstChildElement(), l_id, l_name, l_type );
    //         } else {
    //             throw ExceptionChimera ( ExceptionCode::READ, "Falha, objeto hierarquia: " + std::string ( l_id ) );
    //         }

    //     } else  {

    //         throw ExceptionChimera ( ExceptionCode::READ, "Falha, objeto desconhecido: " + std::string ( l_nomeElemento ) );

    //     }
    //     _nNode = _nNode->NextSiblingElement();
    // }
}


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