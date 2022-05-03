#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/visible/CameraOrbit.hpp"
#include "chimera/core/visible/Components.hpp"

namespace Chimera {
ColladaCam::~ColladaCam() {}

void ColladaCam::create(Entity& entity, pugi::xml_node nodeCam) { // FIXME: preciso mesmo da entidade ???

    ComponentCamera& cc = entity.addComponent<ComponentCamera>();
    cc.tag.id = nodeCam.attribute("id").value();
    cc.tag.tag = nodeCam.attribute("name").value();
    cc.tag.serial = Collada::getNewSerial();

    float fov, near, far, min, max;

    for (pugi::xml_node node = nodeCam.first_child(); node; node = node.next_sibling()) {
        if (std::string("optics") == node.name()) {
            pugi::xml_node perspective = node.child("technique_common").child("perspective");
            fov = perspective.child("xfov").text().as_float();
            // ratio = perspective.child("aspect_ratio").text().as_float();
            near = perspective.child("znear").text().as_float();
            far = perspective.child("zfar").text().as_float();

        } else if (std::string("extra") == node.name()) {

            pugi::xml_node orbital = getExtra(node, "orbital");
            if (orbital != nullptr) {

                min = orbital.child("min").text().as_float();
                max = orbital.child("max").text().as_float();

                ComponentTrans& trans = entity.getComponent<ComponentTrans>();
                glm::vec3 pos = trans.trans->getPosition();
                cc.camera = new CameraOrbit(pos, glm::vec3(0.0, 0.0, 1.0), 0.0, 0.0);
                ((CameraOrbit*)cc.camera)->setLimits(min, max);
            }
        }
    }

    ((ICamera3D*)cc.camera)->setFov(fov);
    ((ICamera3D*)cc.camera)->setNear(near);
    ((ICamera3D*)cc.camera)->setFar(far);
}

} // namespace Chimera