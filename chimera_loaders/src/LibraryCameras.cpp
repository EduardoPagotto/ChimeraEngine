#include "LibraryCameras.h"
#include "ExceptionChimera.h"
#include "CameraSpherical.h"

namespace ChimeraLoaders {

LibraryCameras::LibraryCameras(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryCameras::~LibraryCameras() {
}

Chimera::Camera *LibraryCameras::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_cameras");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("camera");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    tinyxml2::XMLElement* l_pTechnique = findExtra(l_nNodeBase);
                    if (l_pTechnique != nullptr) {

                        tinyxml2::XMLElement* l_nOrbital = l_pTechnique->FirstChildElement ( "orbital" );
                        if ( l_nOrbital != nullptr ) {

                            Chimera::CameraSpherical *pCameraNew = new Chimera::CameraSpherical( l_id );

                            tinyxml2::XMLElement* l_param = l_nOrbital->FirstChildElement();
                            while ( l_param != nullptr ) {

                                if ( strcmp ( l_param->Value(), ( const char* ) "min" ) == 0 ) {

                                    pCameraNew->setDistanciaMinima ( atof ( l_param->GetText() ) );

                                } else if ( strcmp ( l_param->Value(), ( const char* ) "max" ) == 0 ) {

                                    pCameraNew->setDistanciaMaxima ( atof ( l_param->GetText() ) );

                                }

                                l_param = l_param->NextSiblingElement();
                            }

                            //Carrega dados basicos de camera
                            loadbase(l_nNodeBase, pCameraNew);

                            return pCameraNew;

                        }
                        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "orbital nao definido, nao encontrado camera");    
                    }

                    throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "extra, nao encontrado camera");        
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "camera" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_cameras" ) );
    }
}

void LibraryCameras::loadbase ( tinyxml2::XMLElement* _nNode, Chimera::Camera *_pCamera) {

    tinyxml2::XMLElement *l_nPerspective = _nNode->FirstChildElement ( "optics" )->FirstChildElement ( "technique_common" )->FirstChildElement ( "perspective" );
    if ( l_nPerspective != nullptr ) {
        _pCamera->setPerspective ( true );
        _pCamera->setFov ( atof ( l_nPerspective->FirstChildElement ( "xfov" )->GetText() ) );
        _pCamera->setNear ( atof ( l_nPerspective->FirstChildElement ( "znear" )->GetText() ) );
        _pCamera->setFar ( atof ( l_nPerspective->FirstChildElement ( "zfar" )->GetText() ) );

    } else {
        //TODO testar ecarregar ortogonal aqui
    }
}


}