#include "chimera_collada/ColladaRender.hpp"
#include "chimera_collada/ColladaCam.hpp"
#include "chimera_collada/ColladaEffect.hpp"
#include "chimera_ecs/CameraComponent.hpp"
#include "chimera_ecs/Entity.hpp"
#include "chimera_ecs/ShaderComponent.hpp"

namespace ce {

    void colladaRenderLoad(std::shared_ptr<entt::registry> registry, ColladaDom& dom) {

        pugi::xml_node vs = dom.root.child("scene");
        if (const pugi::xml_node extra = vs.child("extra"); extra != nullptr) {

            if (const pugi::xml_node n_tiles = getExtra(extra, "tiles"); n_tiles != nullptr) {

                for (pugi::xml_node n_tile = n_tiles.first_child(); n_tile; n_tile = n_tile.next_sibling()) {

                    Entity entity = Entity::create(registry.get(), n_tile.attribute("name").value(),
                                                   n_tile.attribute("id").value());

                    for (pugi::xml_node node = n_tile.first_child(); node; node = node.next_sibling()) {

                        std::string url = node.attribute("url").value();
                        if (std::string name = node.name(); name == "instance_camera") {

                            ColladaCam cc(registry, dom, url);
                            cc.create(entity, cc.get_library("library_cameras"));
                            cc.create_extra(entity, node.first_child());

                        } else if (name == "instance_effect") {

                            std::string ref_name = node.child("technique_hint").attribute("ref").value();
                            ColladaEffect cs(registry, dom, url);
                            cs.create(ref_name, entity, cs.get_library("library_effects"));
                        }
                    }

                    [[maybe_unused]]
                    CameraComponent& c_cam = entity.get_component<CameraComponent>(registry.get());
                    auto& shader_com = entity.get_component<ShaderComponent>(registry.get());
                    std::shared_ptr<Shader> shader = shader_com.shader;
                }
            }
        }
    }
} // namespace ce
