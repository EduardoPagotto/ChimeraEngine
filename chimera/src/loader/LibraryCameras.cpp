#include "LibraryCameras.hpp"
//#include "chimera/core/CameraFPS.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/render/CameraOrbit.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

LibraryCameras::LibraryCameras(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity) : Library(_root, _url) {
    this->entity = entity;
}

LibraryCameras::~LibraryCameras() {}

void LibraryCameras::target() {

    tinyxml2::XMLElement* l_nCam = root->FirstChildElement("library_cameras")->FirstChildElement("camera");
    for (l_nCam; l_nCam; l_nCam = l_nCam->NextSiblingElement()) {

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

                // TODO: SEra removido na proxima versao
                // posicao da camera desconhecida
                CameraOrbit* cam = new CameraOrbit(glm::vec3(100.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), 0.0, 0.0);
                // CameraFPS* cam = new CameraFPS(glm::vec3(100.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), 0.0, 0.0);
                cam->setLimits(atof(l_nMin->GetText()), atof(l_nMax->GetText()));
                loadbase(l_nCam, cam);
            }
            return;
        }
    }
    throw Exception("Camera nao encontrada: " + url);
}

void LibraryCameras::loadbase(tinyxml2::XMLElement* _nNode, ICamera3D* cam) {
    tinyxml2::XMLElement* l_nPerspective =
        _nNode->FirstChildElement("optics")->FirstChildElement("technique_common")->FirstChildElement("perspective");
    if (l_nPerspective != nullptr) {
        //_pCamera->setPerspective(true);
        // ViewPoint* vp = _pCamera->getViewPoint();
        float fov = atof(l_nPerspective->FirstChildElement("xfov")->GetText());
        float near = atof(l_nPerspective->FirstChildElement("znear")->GetText());
        float far = atof(l_nPerspective->FirstChildElement("zfar")->GetText());

        // ICamera3D* pc = (ICamera3D*)_pCamera->getCamera();
        cam->setFov(fov);
        cam->setNear(near);
        cam->setFar(far);

    } else {
        // TODO testar ecarregar ortogonal aqui
        throw Exception("Camera, Ortogonal nao implementada: " + url);
    }
}
} // namespace Chimera
