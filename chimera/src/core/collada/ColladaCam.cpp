#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/visible/CameraOrthographic.hpp"
#include "chimera/core/visible/ITrans.hpp"

namespace Chimera {
ColladaCam::~ColladaCam() {}

void ColladaCam::create(Entity& entity, pugi::xml_node nodeCam) { // FIXME: preciso mesmo da entidade ???

    CameraComponent& cc = entity.addComponent<CameraComponent>();
    cc.tag.id = nodeCam.attribute("id").value();
    cc.tag.tag = nodeCam.attribute("name").value();
    cc.tag.serial = Collada::getNewSerial();

    float fov, znear, zfar, min, max, xmag, ymag;

    for (pugi::xml_node node = nodeCam.first_child(); node; node = node.next_sibling()) {
        if (std::string("optics") == node.name()) {

            const pugi::xml_node nCamType = node.child("technique_common").first_child();
            if (std::string("perspective") == nCamType.name()) {

                fov = nCamType.child("xfov").text().as_float();
                znear = nCamType.child("znear").text().as_float();
                zfar = nCamType.child("zfar").text().as_float();

            } else if (std::string("orthographic") == nCamType.name()) {

                xmag = nCamType.child("xmag").text().as_float();
                ymag = nCamType.child("ymag").text().as_float();
                znear = nCamType.child("znear").text().as_float();
                zfar = nCamType.child("zfar").text().as_float();

                cc.camera = new CameraOrthographic(xmag, ymag, znear, zfar); // FIXME: 512 ????
            }

        } else if (std::string("extra") == node.name()) {

            pugi::xml_node orbital = getExtra(node, "orbital");
            if (orbital != nullptr) {

                TransComponent& trans = entity.getComponent<TransComponent>();

                cc.camera = new Camera(new Perspective(fov, znear, zfar));
                cc.camera->setPosition(trans.trans->getPosition()); // Assim mesmo ou do campo como abaixo ???
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

                cc.camera = new Camera(new Perspective(fov, znear, zfar));
                cc.camera->setPosition(textToVec3(nFPS.child("pos").text().as_string()));
                cc.camKind = CamKind::FPS;
                setChildParam(nFPS, "up", cc.up);
                setChildParam(nFPS, "yaw", cc.yaw);
                setChildParam(nFPS, "pitch", cc.pitch);
                setChildParam(nFPS, "primary", cc.primary);
                setChildParam(nFPS, "fixedAspectRatio", cc.fixedAspectRatio);
            }
        }
    }
}

} // namespace Chimera