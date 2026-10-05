#include "chimera_collada/ColladaCam.hpp"
#include "chimera_ecs/CameraComponent.hpp"
#include "chimera_ecs/TransComponent.hpp"

namespace ce {
    void ColladaCam::create_extra(Entity& entity, pugi::xml_node node) { // FIXME: remover entity e usar o serviceLoc

        std::string nn = node.name();

        CameraComponent& cc = entity.get_component<CameraComponent>(registry.get());
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

        if (pugi::xml_node n_fps = getExtra(node, "FPS"); n_fps != nullptr) {

            cc.camKind = CamKind::FPS;
            setChildParam(n_fps, "up", cc.up);
            setChildParam(n_fps, "yaw", cc.yaw);
            setChildParam(n_fps, "pitch", cc.pitch);
            setChildParam(n_fps, "primary", cc.primary);
            setChildParam(n_fps, "fixedAspectRatio", cc.fixedAspectRatio);
        }

        if (pugi::xml_node n_static = getExtra(node, "static"); n_static != nullptr) {

            cc.camKind = CamKind::STATIC;
            setChildParam(n_static, "primary", cc.primary);
            setChildParam(n_static, "fixedAspectRatio", cc.fixedAspectRatio);
        }
    }

    void ColladaCam::create(Entity& entity, pugi::xml_node node_cam) {

        CameraComponent& cc = entity.add_component<CameraComponent>(registry.get());
        cc.tag.id = node_cam.attribute("id").value();
        cc.tag.name = node_cam.attribute("name").value();
        // cc.tag.serial = Collada::getNewSerial();

        for (pugi::xml_node node = node_cam.first_child(); node; node = node.next_sibling()) {
            if (std::string("optics") == node.name()) {

                float znear = 0.5f, zfar = 1000.0f;
                const pugi::xml_node n_cam_type = node.child("technique_common").first_child();
                if (std::string("perspective") == n_cam_type.name()) {

                    float xfov = 45.0f;
                    setChildParam(n_cam_type, "xfov", xfov);
                    setChildParam(n_cam_type, "znear", znear);
                    setChildParam(n_cam_type, "zfar", zfar);
                    cc.camera = std::make_shared<CameraPerspective>(xfov, znear, zfar);

                } else if (std::string("orthographic") == n_cam_type.name()) {

                    float xmag = 512.0f, ymag = 512.0f;
                    setChildParam(n_cam_type, "xmag", xmag);
                    setChildParam(n_cam_type, "ymag", ymag);
                    setChildParam(n_cam_type, "znear", znear);
                    setChildParam(n_cam_type, "zfar", zfar);
                    cc.camera = std::make_shared<CameraOrtho>(xmag, ymag, znear, zfar);
                }

                if (entity.has_component<TransComponent>(registry.get())) {
                    TransComponent& trans = entity.get_component<TransComponent>(registry.get());
                    cc.camera->set_position(trans.trans->get_position());
                }
            }
        }
    }

} // namespace ce
