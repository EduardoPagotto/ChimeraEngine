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

        std::string name = n.name();
        std::string url = n.attribute("url").value();
        if (name == "matrix") {

            std::string sid = n.attribute("sid").value();
            if (sid == "transform") {
                ComponentTrans& tc = entity.addComponent<ComponentTrans>();
                tc.trans = new Transform(textToMat4(n.text().as_string()));
            }

        } else if (name == "instance_geometry") {

            ColladaGeometry cg(colladaDom, url);
            cg.create(entity, cg.getLibrary("library_geometries", url));

            pugi::xml_node instanceMaterial = n.child("bind_material").child("technique_common").child("instance_material");
            if (instanceMaterial != nullptr) {
                ColladaMaterial cm(colladaDom, url);
                cm.create(entity, instanceMaterial);
            }

        } else if (name == "instance_light") {

            ColladaLight cl(colladaDom, url);
            cl.create(entity, cl.getLibrary("library_lights", url));

        } else if (name == "instance_node") {
            // pugi::xml_node geo = urlRoot(nodeParent, "library_nodes", nodeParent.attribute("url").value());
            // TODO: implementear para node externo para nodes (particula no teste)
        } else if (name == "instance_camera") {

            ColladaCam cc(colladaDom, url);
            cc.create(entity, cc.getLibrary("library_cameras", url));
        } else if (name == "node") {
            // TODO: implementar hierarquia
        }
    }
}

void ColladaVisualScene::loadAll(pugi::xml_node node, Registry* reg) {

    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling())
        loadNode(n, reg);
}

} // namespace Chimera