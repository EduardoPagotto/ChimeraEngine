#include "chimera_collada/ColladaVisualScene.hpp"
#include "chimera_collada/ColladaCam.hpp"
#include "chimera_collada/ColladaGeometry.hpp"
#include "chimera_collada/ColladaLight.hpp"
#include "chimera_collada/ColladaMaterial.hpp"
#include "chimera_ecs/Entity.hpp"
#include "chimera_ecs/TransComponent.hpp"

namespace ce {

    void ColladaVisualScene::load_node(pugi::xml_node node) {

        std::string ent_name = node.attribute("name").value();
        std::string ent_id = node.attribute("id").value();
        Entity entity = Entity::create(registry.get(), ent_name, ent_id);
        for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling())
            node_data(n, entity);
    }

    void ColladaVisualScene::node_data(pugi::xml_node n, Entity entity) {

        std::string name = n.name();
        std::string url = n.attribute("url").value();
        if (name == "matrix") {

            if (std::string sid = n.attribute("sid").value(); sid == "transform") {
                TransComponent& tc = entity.add_component<TransComponent>(registry.get());
                tc.trans = new Transform(textToMat4(n.text().as_string()));
            }

        } else if (name == "instance_geometry") {

            ColladaGeometry cg(registry, colladaDom, url);
            cg.create(entity, cg.get_library("library_geometries"));

            if (const pugi::xml_node n_mat = n.child("bind_material"); n_mat) {
                if (const pugi::xml_node instance_material = n_mat.child("technique_common").child("instance_material");
                    instance_material != nullptr) {
                    std::string target = instance_material.attribute("target").value();
                    ColladaMaterial cm(registry, colladaDom, target);
                    cm.create(entity, cm.get_library("library_materials"));
                }
            }

        } else if (name == "instance_light") {

            ColladaLight cl(registry, colladaDom, url);
            cl.create(entity, cl.get_library("library_lights"));

        } else if (name == "instance_node") {

            // TODO: refazer para uso de hierarquia
            // ColladaVisualScene vs(colladaDom, url);
            // const pugi::xml_node nNodes = vs.getLibrary("library_nodes");
            // for (pugi::xml_node n = nNodes.first_child(); n; n = n.next_sibling()) {
            //     vs.nodeData(n, entity);
            // }

        } else if (name == "instance_camera") {

            ColladaCam cc(registry, colladaDom, url);
            cc.create(entity, cc.get_library("library_cameras"));
            cc.create_extra(entity, n.first_child());

        } else if (name == "node") {
            // TODO: implementar hierarquia
        }
    }

    void ColladaVisualScene::load_all(pugi::xml_node node) {

        for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling())
            load_node(n);
    }

} // namespace ce
