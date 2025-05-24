#include "chimera/collada/ColladaCam.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/ITrans.hpp"

namespace ce {
    void ColladaCam::createExtra(Entity& entity, pugi::xml_node node) { // FIXME: remover entity e usar o serviceLoc

        std::string nn = node.name();

        CameraComponent& cc = entity.getComponent<CameraComponent>();
        if (pugi::xml_node orbital = getExtra(node, "orbital"); orbital != nullptr) {

            cc.camKind = CamKind::ORBIT;
            setChildParam(orbital, "up", cc.up);
            setChildParam(orbital, "yaw", cc.yaw);
            setChildParam(orbital, "pitch", cc.pitch);
            setChildParam(orbital, "min", cc.min);
            setChildParam(orbital, "max", cc.max);
            setChildParam(orbital, "primary", cc.primary);
            setChildParam(orbital, "fixedAspectRatio", cc.fixedAspectRatio);
        }

        if (pugi::xml_node nFPS = getExtra(node, "FPS"); nFPS != nullptr) {

            cc.camKind = CamKind::FPS;
            setChildParam(nFPS, "up", cc.up);
            setChildParam(nFPS, "yaw", cc.yaw);
            setChildParam(nFPS, "pitch", cc.pitch);
            setChildParam(nFPS, "primary", cc.primary);
            setChildParam(nFPS, "fixedAspectRatio", cc.fixedAspectRatio);
        }

        if (pugi::xml_node nStatic = getExtra(node, "static"); nStatic != nullptr) {

            cc.camKind = CamKind::STATIC;
            setChildParam(nStatic, "primary", cc.primary);
            setChildParam(nStatic, "fixedAspectRatio", cc.fixedAspectRatio);
        }
    }

    void ColladaCam::create(Entity& entity, pugi::xml_node nodeCam) {

        CameraComponent& cc = entity.addComponent<CameraComponent>();
        cc.tag.id = nodeCam.attribute("id").value();
        cc.tag.name = nodeCam.attribute("name").value();
        // cc.tag.serial = Collada::getNewSerial();

        for (pugi::xml_node node = nodeCam.first_child(); node; node = node.next_sibling()) {
            if (std::string("optics") == node.name()) {

                float znear = 0.5f, zfar = 1000.0f;
                const pugi::xml_node nCamType = node.child("technique_common").first_child();
                if (std::string("perspective") == nCamType.name()) {

                    float xfov = 45.0f;
                    setChildParam(nCamType, "xfov", xfov);
                    setChildParam(nCamType, "znear", znear);
                    setChildParam(nCamType, "zfar", zfar);
                    cc.camera = new CameraPerspective(xfov, znear, zfar);

                } else if (std::string("orthographic") == nCamType.name()) {

                    float xmag = 512.0f, ymag = 512.0f;
                    setChildParam(nCamType, "xmag", xmag);
                    setChildParam(nCamType, "ymag", ymag);
                    setChildParam(nCamType, "znear", znear);
                    setChildParam(nCamType, "zfar", zfar);
                    cc.camera = new CameraOrtho(xmag, ymag, znear, zfar);
                }

                if (entity.hasComponent<TransComponent>()) {
                    TransComponent& trans = entity.getComponent<TransComponent>();
                    cc.camera->setPosition(trans.trans->getPosition());
                }
            }
        }
    }

} // namespace ce
