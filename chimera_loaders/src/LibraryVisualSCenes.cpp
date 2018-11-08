#include "LibraryVisualSCenes.h"
#include "ExceptionChimera.h"
#include "LoaderDaeUtils.h"

#include "LibraryCameras.h"
#include "LibraryLights.h"
#include "LibraryGeometrys.h"

#include "Transform.h"

namespace ChimeraLoaders {

LibraryVisualScenes::LibraryVisualScenes(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryVisualScenes::~LibraryVisualScenes() {
}

Chimera::Group *LibraryVisualScenes::target() {

    tinyxml2::XMLElement* l_nScene = root->FirstChildElement("library_visual_scenes")->FirstChildElement ("visual_scene");
    for(l_nScene; l_nScene; l_nScene=l_nScene->NextSiblingElement()) {

        std::string l_id = l_nScene->Attribute ( "id" );
        if (url.compare(l_id) == 0) {

            Chimera::Group *pRootNode = new Chimera::Group(nullptr, l_id );
            tinyxml2::XMLElement* l_nNode = l_nScene->FirstChildElement ("node");
            for(l_nNode; l_nNode; l_nNode=l_nNode->NextSiblingElement()) {

                const char * l_idR = l_nNode->Attribute ( "id" );
                const char * l_name = l_nNode->Attribute ( "name" );
                const char * l_type = l_nNode->Attribute ( "type" );

                tinyxml2::XMLElement* l_nDadoNode = l_nNode->FirstChildElement();
                if ( l_nDadoNode != nullptr ) {
                    carregaNode ( (Chimera::Node*)pRootNode, l_nDadoNode, l_idR, l_name, l_type );
                } 
            }
            return pRootNode;
        }
    }

    throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Visual scenes nao encontrado: " + url );
}


glm::mat4  LibraryVisualScenes::getTransformation(tinyxml2::XMLElement* _nNode) {

    glm::mat4 l_pTransform;

    const char* l_tipoTransform = _nNode->Attribute ( "sid" );
    if ( strcmp ( l_tipoTransform, ( const char* ) "transform" ) == 0 ) {
        l_pTransform = Chimera::loadTransformMatrix ( _nNode->GetText() );
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Matrix de transformacao invalida" );
        //TODO: implementar carga de posicao, rotacao e transformar em matricial em l_pTransform
    }

    return l_pTransform;
}

void LibraryVisualScenes::carregaNode ( Chimera::Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type ) {

    glm::mat4 l_pTransform;
    Chimera::Node *pLastNodeDone = nullptr;

    for(_nNode; _nNode; _nNode=_nNode->NextSiblingElement()){

        tinyxml2::XMLElement* l_nNodeSourceData = nullptr;

        const char* l_url = _nNode->Attribute ( "url" );
        const char* l_target = _nNode->Attribute ( "target" );
        const char* l_nomeElemento = _nNode->Value();

        if ( strcmp ( l_nomeElemento, ( const char* ) "matrix" ) == 0 ) {

            l_pTransform = getTransformation(_nNode);

        } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_camera" ) == 0 ) {

            LibraryCameras lib(root, l_url);
            Chimera::Camera *pCamera = lib.target();

            pCamera->setTransform ( l_pTransform );

            _pNodePai->addChild ( pCamera );
            pLastNodeDone = pCamera;

         } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_light" ) == 0 ) {

            LibraryLights lib(root, l_url);
            Chimera::Light *pLight = lib.target();

            pLight->setTransform ( l_pTransform );

            _pNodePai->addChild ( pLight );
            pLastNodeDone = pLight;

         } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_geometry" ) == 0 ) {

             LibraryGeometrys lib(root, l_url);
             Chimera::Draw *pDraw = lib.target();

            //FIXME: quando uar solido, como contornar??
            Chimera::Transform *pTrans = new Chimera::Transform(_pNodePai, _id);
            pTrans->setMatrix(l_pTransform);
            pTrans->addChild(pDraw);
            pLastNodeDone = pTrans;

         } else if ( strcmp ( l_nomeElemento, ( const char* ) "node" ) == 0 ) {

            const char * l_id = _nNode->Attribute ( "id" );
            const char * l_name = _nNode->Attribute ( "name" );
            const char * l_type = _nNode->Attribute ( "type" );

            if ( pLastNodeDone != nullptr ) {
                carregaNode ( pLastNodeDone, _nNode->FirstChildElement(), l_id, l_name, l_type );
            } else {
                throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, objeto hierarquia: " + std::string ( l_id ) );
            }

        } else  {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, objeto desconhecido: " + std::string ( l_nomeElemento ) );
        }
    }
}

}