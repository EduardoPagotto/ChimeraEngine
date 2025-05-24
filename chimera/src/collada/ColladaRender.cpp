#include "chimera/collada/ColladaRender.hpp"
#include "chimera/collada/ColladaCam.hpp"
#include "chimera/collada/ColladaEffect.hpp"
#include "chimera/collada/ColladaVisualScene.hpp"
#include "chimera/core/gl/Shader.hpp"
#include "chimera/ecs/CameraComponent.hpp"

namespace ce {

    void colladaRenderLoad(ColladaDom& dom) {

        pugi::xml_node vs = dom.root.child("scene");
        if (const pugi::xml_node extra = vs.child("extra"); extra != nullptr) {

            if (const pugi::xml_node nTiles = getExtra(extra, "tiles"); nTiles != nullptr) {

                for (pugi::xml_node nTile = nTiles.first_child(); nTile; nTile = nTile.next_sibling()) {

                    Entity entity(nTile.attribute("name").value(), nTile.attribute("id").value());

                    for (pugi::xml_node node = nTile.first_child(); node; node = node.next_sibling()) {

                        std::string url = node.attribute("url").value();
                        if (std::string name = node.name(); name == "instance_camera") {

                            ColladaCam cc(dom, url);
                            cc.create(entity, cc.getLibrary("library_cameras"));
                            cc.createExtra(entity, node.first_child());

                        } else if (name == "instance_effect") {

                            std::string refName = node.child("technique_hint").attribute("ref").value();
                            ColladaEffect cs(dom, url);
                            cs.create(refName, entity, cs.getLibrary("library_effects"));
                        }
                    }

                    [[maybe_unused]]
                    CameraComponent& cCam = entity.getComponent<CameraComponent>();
                    auto& shaderCom = entity.getComponent<ShaderComponent>();
                    std::shared_ptr<Shader> shader = shaderCom.shader;
                }
            }
        }
    }
} // namespace ce
