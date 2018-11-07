#include "LibraryCameras.h"
#include "ExceptionChimera.h"
#include "CameraSpherical.h"

namespace ChimeraLoaders {

LibraryCameras::LibraryCameras(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryCameras::~LibraryCameras() {
}

Chimera::Camera *LibraryCameras::target() {

    tinyxml2::XMLElement* l_nCam = root->FirstChildElement("library_cameras")->FirstChildElement ("camera");
    for (l_nCam; l_nCam; l_nCam = l_nCam->NextSiblingElement()) {

        std::string l_id = l_nCam->Attribute ( "id" );
        if (url.compare(l_id) == 0) {

            tinyxml2::XMLElement* l_param = findExtra(l_nCam)->FirstChildElement("orbital")->FirstChildElement();

            Chimera::CameraSpherical *pCameraNew = new Chimera::CameraSpherical( l_id );

            for (l_param; l_param; l_param->NextSiblingElement()) {

                const char *parametro = l_param->Value();
                if ( strcmp(parametro,(const char*)"min") == 0 ) 
                    pCameraNew->setDistanciaMinima(atof(l_param->GetText()));
                else if ( strcmp (parametro, ( const char* ) "max" ) == 0 )
                    pCameraNew->setDistanciaMaxima (atof(l_param->GetText()));

            }

            //Carrega dados basicos de camera
            loadbase(l_nCam, pCameraNew);
            return pCameraNew;
        }
    }
    throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Camera, nao encontrada: " + url);
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
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Camera, Ortogonal nao implementada: " + url);
    }
}
}