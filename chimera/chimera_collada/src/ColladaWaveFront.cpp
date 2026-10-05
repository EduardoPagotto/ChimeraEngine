#include "chimera_collada/ColladaWaveFront.hpp"
#include "chimera_collada/ColladaEffect.hpp"
#include "chimera_core/gl/Wavefront.hpp"
#include "chimera_ecs/MaterialComponent.hpp"
#include "chimera_ecs/MeshComponent.hpp"

namespace ce {
    void ColladaWaveFront::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo) {

        MeshComponent& e_mesh = entity.add_component<MeshComponent>(registry.get());
        e_mesh.mesh = new Mesh();
        e_mesh.tag.id = id;
        e_mesh.tag.name = name;
        // eMesh.tag.serial = Collada::getNewSerial();
        e_mesh.type = getMeshTypeFromString(geo.attribute("partition").value());
        std::string target = geo.attribute("target").value();

        MaterialComponent& e_material = entity.add_component<MaterialComponent>(registry.get());
        e_material.tag.id = e_mesh.tag.id + "_mat";
        e_material.tag.name = e_mesh.tag.name + "_mat";
        e_material.material = std::make_shared<Material>();

        WaveFront wf(registry);

        std::string mat_file;
        wf.wavefront_obj_load(target, e_mesh.mesh, mat_file);
        if (mat_file.size() > 0) {
            wf.wavefront_mtl_load(mat_file, e_material.material);
        }

        if (pugi::xml_node n_shade = geo.next_sibling(); n_shade) {
            if (pugi::xml_node technique_hint = n_shade.child("technique_hint"); technique_hint != nullptr) {
                if (std::string(technique_hint.attribute("profile").value()) == "GLSL") {
                    std::string ref_name = technique_hint.attribute("ref").value();
                    std::string url = n_shade.attribute("url").value();
                    ColladaEffect cf(registry, colladaDom, url);

                    cf.create(ref_name, entity, cf.get_library("library_effects"));
                }
            }
        }
    }
} // namespace ce
