#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/ITrans.hpp"

namespace Chimera {
ColladaCam::~ColladaCam() {}

void ColladaCam::createExtra(Entity& entity, pugi::xml_node node) {

    std::string nn = node.name();

    CameraComponent& cc = entity.getComponent<CameraComponent>();
    pugi::xml_node orbital = getExtra(node, "orbital");
    if (orbital != nullptr) {

        cc.camKind = CamKind::ORBIT;
        setChildParam(orbital, "up", cc.up);
        setChildParam(orbital, "yaw", cc.yaw);
        setChildParam(orbital, "pitch", cc.pitch);
        setChildParam(orbital, "min", cc.min);
        setChildParam(orbital, "max", cc.max);
        setChildParam(orbital, "primary", cc.primary);
        setChildParam(orbital, "fixedAspectRatio", cc.fixedAspectRatio);
    }

    pugi::xml_node nFPS = getExtra(node, "FPS");
    if (nFPS != nullptr) {

        cc.camKind = CamKind::FPS;
        setChildParam(nFPS, "up", cc.up);
        setChildParam(nFPS, "yaw", cc.yaw);
        setChildParam(nFPS, "pitch", cc.pitch);
        setChildParam(nFPS, "primary", cc.primary);
        setChildParam(nFPS, "fixedAspectRatio", cc.fixedAspectRatio);
    }

    pugi::xml_node nStatic = getExtra(node, "static");
    if (nStatic != nullptr) {

        cc.camKind = CamKind::STATIC;
        setChildParam(nStatic, "primary", cc.primary);
        setChildParam(nStatic, "fixedAspectRatio", cc.fixedAspectRatio);
    }
}

void ColladaCam::create(Entity& entity, pugi::xml_node nodeCam) { // FIXME: preciso mesmo da entidade ???

    CameraComponent& cc = entity.addComponent<CameraComponent>();
    cc.tag.id = nodeCam.attribute("id").value();
    cc.tag.tag = nodeCam.attribute("name").value();
    cc.tag.serial = Collada::getNewSerial();

    for (pugi::xml_node node = nodeCam.first_child(); node; node = node.next_sibling()) {
        if (std::string("optics") == node.name()) {

            float znear = 0.5f, zfar = 1000.0f;
            const pugi::xml_node nCamType = node.child("technique_common").first_child();
            if (std::string("perspective") == nCamType.name()) {

                float xfov = 45.0f;
                setChildParam(nCamType, "xfov", xfov);
                setChildParam(nCamType, "znear", znear);
                setChildParam(nCamType, "zfar", zfar);
                cc.camera = new Camera(xfov, znear, zfar);

            } else if (std::string("orthographic") == nCamType.name()) {

                float xmag = 512.0f, ymag = 512.0f;
                setChildParam(nCamType, "xmag", xmag);
                setChildParam(nCamType, "ymag", ymag);
                setChildParam(nCamType, "znear", znear);
                setChildParam(nCamType, "zfar", zfar);
                cc.camera = new Camera(xmag, ymag, znear, zfar);
            }

            if (entity.hasComponent<TransComponent>()) {
                TransComponent& trans = entity.getComponent<TransComponent>();
                cc.camera->setPosition(trans.trans->getPosition());
            }
        }
    }
}

} // namespace Chimera