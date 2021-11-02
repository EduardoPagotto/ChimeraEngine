#include "LibraryCameras.hpp"
//#include "chimera/core/CameraFPS.hpp"
#include "chimera/core/CameraOrbit.hpp"
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
            // loadbase(l_nCam, pCameraNew);
            tinyxml2::XMLElement* l_nExtra = findExtra(l_nCam);
            if (l_nExtra) {
                tinyxml2::XMLElement* l_nMin = l_nExtra->FirstChildElement("orbital")->FirstChildElement("min");
                tinyxml2::XMLElement* l_nMax = l_nExtra->FirstChildElement("orbital")->FirstChildElement("max");

                // posicao da camera desconhecida
                Chimera::CameraOrbit* cam = new Chimera::CameraOrbit(glm::vec3(100.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), 0.0, 0.0);
                // Chimera::CameraFPS* cam = new Chimera::CameraFPS(glm::vec3(100.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), 0.0, 0.0);
                cam->setLimits(atof(l_nMin->GetText()), atof(l_nMax->GetText()));
                pCameraNew->setCamera(cam);
                loadbase(l_nCam, pCameraNew);
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
        // Chimera::ViewPoint* vp = _pCamera->getViewPoint();
        float fov = atof(l_nPerspective->FirstChildElement("xfov")->GetText());
        float near = atof(l_nPerspective->FirstChildElement("znear")->GetText());
        float far = atof(l_nPerspective->FirstChildElement("zfar")->GetText());

        Chimera::ICamera3D* pc = (Chimera::ICamera3D*)_pCamera->getCamera();
        pc->setFov(fov);
        pc->setNear(near);
        pc->setFar(far);

    } else {
        // TODO testar ecarregar ortogonal aqui
        throw Chimera::Exception("Camera, Ortogonal nao implementada: " + url);
    }
}
} // namespace ChimeraLoaders
