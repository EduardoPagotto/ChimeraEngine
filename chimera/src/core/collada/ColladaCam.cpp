#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/visible/CameraOrbit.hpp"
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

                min = orbital.child("min").text().as_float();
                max = orbital.child("max").text().as_float();

                TransComponent& trans = entity.getComponent<TransComponent>();
                glm::vec3 pos = trans.trans->getPosition();

                cc.camera = new CameraOrbit(pos, glm::vec3(0.0, 0.0, 1.0), 0.0, 0.0);
                ((CameraOrbit*)cc.camera)->setLimits(min, max);

                ((ICamera3D*)cc.camera)->setFov(fov);
                ((ICamera3D*)cc.camera)->setNear(znear);
                ((ICamera3D*)cc.camera)->setFar(zfar);
            }
        }
    }
}

} // namespace Chimera