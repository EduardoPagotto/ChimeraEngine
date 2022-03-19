#include "LibraryCameras.hpp"
//#include "chimera/core/CameraFPS.hpp"
#include "chimera/core/visible/CameraOrbit.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void LibraryCameras::target() {

    for (tinyxml2::XMLElement* l_nCam = root->FirstChildElement("library_cameras")->FirstChildElement("camera"); l_nCam;
         l_nCam = l_nCam->NextSiblingElement()) {

        std::string l_id = l_nCam->Attribute("id");
        if (url.compare(l_id) == 0) {
            tinyxml2::XMLElement* l_nExtra = findExtra(l_nCam);
            if (l_nExtra) {
                tinyxml2::XMLElement* l_nMin = l_nExtra->FirstChildElement("orbital")->FirstChildElement("min");
                tinyxml2::XMLElement* l_nMax = l_nExtra->FirstChildElement("orbital")->FirstChildElement("max");

                auto& tag = entity.getComponent<TagComponent>();
                tag.tag = l_id;

                CameraComponent& cc = entity.addComponent<CameraComponent>();
                // cc.camera = new CameraOrbit(glm::vec3(0.0, 200.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
                CameraOrbit* camZ = new CameraOrbit(glm::vec3(100.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), 0.0, 0.0);
                cc.camera = camZ;
                camZ->setLimits(atof(l_nMin->GetText()), atof(l_nMax->GetText()));
                loadbase(l_nCam, camZ);
            }
            return;
        }
    }
    throw std::string("Camera nao encontrada: " + url);
}

void LibraryCameras::loadbase(tinyxml2::XMLElement* _nNode, ICamera3D* cam) {
    tinyxml2::XMLElement* l_nPerspective =
        _nNode->FirstChildElement("optics")->FirstChildElement("technique_common")->FirstChildElement("perspective");
    if (l_nPerspective != nullptr) {
        float fov = atof(l_nPerspective->FirstChildElement("xfov")->GetText());
        float near = atof(l_nPerspective->FirstChildElement("znear")->GetText());
        float far = atof(l_nPerspective->FirstChildElement("zfar")->GetText());

        cam->setFov(fov);
        cam->setNear(near);
        cam->setFar(far);

    } else {
        // TODO testar ecarregar ortogonal aqui
        throw std::string("Camera, Ortogonal nao implementada: " + url);
    }
}
} // namespace Chimera
