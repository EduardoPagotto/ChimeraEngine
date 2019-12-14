#include "LibraryCameras.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/node/NodeCamera.hpp"

namespace ChimeraLoaders {

LibraryCameras::LibraryCameras(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}

LibraryCameras::~LibraryCameras() {}

Chimera::NodeCamera* LibraryCameras::target() {

    tinyxml2::XMLElement* l_nCam = root->FirstChildElement("library_cameras")->FirstChildElement("camera");
    for (l_nCam; l_nCam; l_nCam = l_nCam->NextSiblingElement()) {

        std::string l_id = l_nCam->Attribute("id");
        if (url.compare(l_id) == 0) {

            Chimera::NodeCamera* pCameraNew = new Chimera::NodeCamera(nullptr, l_id);
            loadbase(l_nCam, pCameraNew);

            tinyxml2::XMLElement* l_nExtra = findExtra(l_nCam);
            if (l_nExtra) {
                tinyxml2::XMLElement* l_nMin = l_nExtra->FirstChildElement("orbital")->FirstChildElement("min");
                tinyxml2::XMLElement* l_nMax = l_nExtra->FirstChildElement("orbital")->FirstChildElement("max");

                pCameraNew->createTrackBall();
                pCameraNew->getTrackBall()->setMin(atof(l_nMin->GetText()));
                pCameraNew->getTrackBall()->setMax(atof(l_nMax->GetText()));
            }

            return pCameraNew;
        }
    }
    throw Chimera::Exception("Camera nao encontrada: " + url);
}

void LibraryCameras::loadbase(tinyxml2::XMLElement* _nNode, Chimera::NodeCamera* _pCamera) {
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
