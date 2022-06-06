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
                // ratio = nCamType.child("aspect_ratio").text().as_float();
                znear = nCamType.child("znear").text().as_float();
                zfar = nCamType.child("zfar").text().as_float();

            } else if (std::string("orthographic") == nCamType.name()) {

                xmag = nCamType.child("xmag").text().as_float();
                ymag = nCamType.child("ymag").text().as_float();
                // ratio = nCamType.child("aspect_ratio").text().as_float();
                znear = nCamType.child("znear").text().as_float();
                zfar = nCamType.child("zfar").text().as_float();

                cc.camera = new CameraOrthographic(512.0, znear, zfar); // FIXME: 512 ????
            }

        } else if (std::string("extra") == node.name()) {

            pugi::xml_node orbital = getExtra(node, "orbital");
            if (orbital != nullptr) {

                TransComponent& trans = entity.getComponent<TransComponent>();

                cc.camera = new Camera(new Perspective(fov, znear, zfar));
                cc.camera->setPosition(trans.trans->getPosition()); // Assim mesmo ou do campo como abaixo ???
                cc.camKind = CamKind::ORBIT;
                cc.up = textToVec3(orbital.child("up").text().as_string());
                cc.yaw = orbital.child("yaw").text().as_float();
                cc.pitch = orbital.child("pitch").text().as_float();
                cc.min = orbital.child("min").text().as_float();
                cc.max = orbital.child("max").text().as_float();
            }

            pugi::xml_node nFPS = getExtra(node, "FPS");
            if (nFPS != nullptr) {

                cc.camera = new Camera(new Perspective(fov, znear, zfar));
                cc.camera->setPosition(textToVec3(nFPS.child("pos").text().as_string()));
                cc.camKind = CamKind::FPS;
                cc.up = textToVec3(nFPS.child("up").text().as_string());
                cc.yaw = nFPS.child("yaw").text().as_float();
                cc.pitch = nFPS.child("pitch").text().as_float();
            }
        }
    }
}

} // namespace Chimera