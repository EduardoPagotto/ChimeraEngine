#include "chimera/colladaDB/ColladaPhysicScene.hpp"
//#include "chimera/"

namespace Chimera {

ColladaPhysicScene::~ColladaPhysicScene() {}

void ColladaPhysicScene::loadNode(pugi::xml_node node, Registry* reg, const std::string& body, const std::string& target) {

    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling()) {

        std::string val = n.name();

        if (val == "matrix") {

            std::string sid = n.attribute("sid").value();
            if (sid == "transform") {
                // ComponentTrans tc = entity.addComponent<ComponentTrans>(new Transform(textToMat4(n.text().as_string())));
                //  tc.trans = new Transform(transLocal);
            }
        }
    }
}

void ColladaPhysicScene::loadAll(pugi::xml_node node, Registry* reg) {

    std::string id = node.attribute("id").value();
    std::string name = node.attribute("name").value();

    // Entity entity = reg->createEntity(name, id);
    // PhysicsControl& pc = entity.addComponent<PhysicsControl>();
    // pc.setGravity(btVector3(l_arrayF[0], l_arrayF[1], l_arrayF[2]));

    pugi::xml_node nTec = node.child("technique_common");
    std::string sGrav = nTec.child("gravity").text().as_string();
    float ts = nTec.child("time_step").text().as_float();

    pugi::xml_node nInstace = node.child("instance_physics_model");

    std::string val = nInstace.name();
    std::string url = nInstace.attribute("url").value();

    for (pugi::xml_node nRb = nInstace.first_child(); nRb; nRb = nRb.next_sibling()) {

        val = nRb.name();
        std::string body = nRb.attribute("body").value();
        std::string target = nRb.attribute("target").value();

        pugi::xml_node models = urlRoot(nInstace, "library_physics_models", url);
        loadNode(models, reg, body, target);
    }
}

} // namespace Chimera