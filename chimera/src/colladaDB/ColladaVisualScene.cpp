#include "chimera/colladaDB/ColladaVisualScene.hpp"
#include "chimera/colladaDB/ColladaCam.hpp"
#include "chimera/colladaDB/ColladaGeometry.hpp"
#include "chimera/colladaDB/ColladaLight.hpp"
#include "chimera/colladaDB/ColladaMaterial.hpp"
#include "chimera/core/visible/Transform.hpp"

namespace Chimera {

ColladaVisualScene::~ColladaVisualScene() {}

void ColladaVisualScene::loadNode(pugi::xml_node node, Registry* reg) {

    Entity entity = reg->createEntity(node.attribute("name").value(), node.attribute("id").value());
    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling()) {

        std::string val = n.name();
        if (val == "matrix") {

            std::string sid = n.attribute("sid").value();
            if (sid == "transform") {
                ComponentTrans& tc = entity.addComponent<ComponentTrans>();
                tc.trans = new Transform(textToMat4(n.text().as_string()));
            }

        } else if (val == "instance_geometry") {

            ColladaGeometry cg;
            cg.create(entity, n);

            pugi::xml_node instanceMaterial = n.child("bind_material").child("technique_common").child("instance_material");
            if (instanceMaterial != nullptr) {
                ColladaMaterial cm;
                cm.create(entity, instanceMaterial);
            }

        } else if (val == "instance_light") {

            ColladaLight cl;
            cl.create(entity, n);

        } else if (val == "instance_node") {

            // pugi::xml_node geo = urlRoot(nodeParent, "library_nodes", nodeParent.attribute("url").value());
            // TODO: implementear para node externo para nodes (particula no teste)

        } else if (val == "instance_camera") {

            ColladaCam cc;
            cc.create(entity, n);
        } else if (val == "node") {
            // TODO: implementar hierarquia
        }
    }
}

void ColladaVisualScene::loadAll(pugi::xml_node node, Registry* reg) {

    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling())
        loadNode(n, reg);
}

} // namespace Chimera