#include "chimera/render/collada/ColladaRender.hpp"
#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/collada/ColladaEffect.hpp"
#include "chimera/core/collada/ColladaVisualScene.hpp"
#include "chimera/core/visible/Shader.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void colladaRenderLoad(ColladaDom& dom) {

    Registry* r = Singleton<Registry>::get();

    pugi::xml_node vs = dom.root.child("scene");
    if (const pugi::xml_node extra = vs.child("extra"); extra != nullptr) {

        if (const pugi::xml_node nTiles = getExtra(extra, "tiles"); nTiles != nullptr) {

            for (pugi::xml_node nTile = nTiles.first_child(); nTile; nTile = nTile.next_sibling()) {

                Entity entity = r->createEntity(nTile.attribute("name").value(), nTile.attribute("id").value());
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

                CameraComponent& cCam = entity.getComponent<CameraComponent>();
                Shader& shader = entity.getComponent<Shader>();
            }
        }
    }

    Singleton<Registry>::release();
}
} // namespace Chimera