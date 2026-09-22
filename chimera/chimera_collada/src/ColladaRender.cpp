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

            if (const pugi::xml_node nTiles = getExtra(extra, "tiles"); nTiles != nullptr) {

                for (pugi::xml_node nTile = nTiles.first_child(); nTile; nTile = nTile.next_sibling()) {

                    Entity entity =
                        Entity::Create(registry.get(), nTile.attribute("name").value(), nTile.attribute("id").value());

                    for (pugi::xml_node node = nTile.first_child(); node; node = node.next_sibling()) {

                        std::string url = node.attribute("url").value();
                        if (std::string name = node.name(); name == "instance_camera") {

                            ColladaCam cc(registry, dom, url);
                            cc.create(entity, cc.getLibrary("library_cameras"));
                            cc.createExtra(entity, node.first_child());

                        } else if (name == "instance_effect") {

                            std::string refName = node.child("technique_hint").attribute("ref").value();
                            ColladaEffect cs(registry, dom, url);
                            cs.create(refName, entity, cs.getLibrary("library_effects"));
                        }
                    }

                    [[maybe_unused]]
                    CameraComponent& cCam = entity.getComponent<CameraComponent>(registry.get());
                    auto& shaderCom = entity.getComponent<ShaderComponent>(registry.get());
                    std::shared_ptr<Shader> shader = shaderCom.shader;
                }
            }
        }
    }
} // namespace ce
