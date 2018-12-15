#include "LibraryCameras.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/node/CameraSpherical.hpp"

namespace ChimeraLoaders {

LibraryCameras::LibraryCameras(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}

LibraryCameras::~LibraryCameras() {}

Chimera::Camera* LibraryCameras::target() {

    tinyxml2::XMLElement* l_nCam = root->FirstChildElement("library_cameras")->FirstChildElement("camera");
    for (l_nCam; l_nCam; l_nCam = l_nCam->NextSiblingElement()) {

        std::string l_id = l_nCam->Attribute("id");
        if (url.compare(l_id) == 0) {

            Chimera::CameraSpherical* pCameraNew = new Chimera::CameraSpherical(l_id);
            loadbase(l_nCam, pCameraNew);

            tinyxml2::XMLElement* l_nExtra = findExtra(l_nCam);
            if (l_nExtra) {
                tinyxml2::XMLElement* l_nMin = l_nExtra->FirstChildElement("orbital")->FirstChildElement("min");
                tinyxml2::XMLElement* l_nMax = l_nExtra->FirstChildElement("orbital")->FirstChildElement("max");
                pCameraNew->setDistanciaMinima(atof(l_nMin->GetText()));
                pCameraNew->setDistanciaMaxima(atof(l_nMax->GetText()));
            }

            return pCameraNew;
        }
    }
    throw Chimera::Exception("Camera nao encontrada: " + url);
}

void LibraryCameras::loadbase(tinyxml2::XMLElement* _nNode, Chimera::Camera* _pCamera) {
    tinyxml2::XMLElement* l_nPerspective =
        _nNode->FirstChildElement("optics")->FirstChildElement("technique_common")->FirstChildElement("perspective");
    if (l_nPerspective != nullptr) {
        //_pCamera->setPerspective(true);
        Chimera::ViewPoint* vp = _pCamera->getViewPoint();
        vp->fov = atof(l_nPerspective->FirstChildElement("xfov")->GetText());
        vp->near = atof(l_nPerspective->FirstChildElement("znear")->GetText());
        vp->far = atof(l_nPerspective->FirstChildElement("zfar")->GetText());

    } else {
        // TODO testar ecarregar ortogonal aqui
        throw Chimera::Exception("Camera, Ortogonal nao implementada: " + url);
    }
}
} // namespace ChimeraLoaders
