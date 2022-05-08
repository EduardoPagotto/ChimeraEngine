#include "chimera/render/collada/ColladaRender.hpp"
#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/collada/ColladaShader.hpp"
#include "chimera/core/collada/ColladaVisualScene.hpp"
#include "chimera/core/visible/Shader.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void colladaRenderLoad(ColladaDom& dom, Registry& r) {

    pugi::xml_node vs = dom.root.child("scene");
    for (pugi::xml_node n = vs.first_child(); n; n = n.next_sibling()) {

        std::string instance = n.name();
        std::string url = n.attribute("url").value();
        if (instance == "instance_visual_scene") {

            ColladaVisualScene vs(dom, url);
            const pugi::xml_node extra = vs.getLibrary("library_visual_scenes").child("extra");
            if (extra != nullptr) {

                const pugi::xml_node nTiles = getExtra(extra, "tiles");
                if (nTiles != nullptr) {
                    for (pugi::xml_node nTile = nTiles.first_child(); nTile; nTile = nTile.next_sibling()) {

                        Entity entity = r.createEntity(nTile.attribute("name").value(), nTile.attribute("id").value());
                        const pugi::xml_node nCam = nTile.child("instance_camera");
                        url = nCam.attribute("url").value();

                        ColladaCam cc(dom, url);
                        cc.create(entity, cc.getLibrary("library_cameras"));

                        CameraComponent& cCam = entity.getComponent<CameraComponent>();

                        const pugi::xml_node nEffect = nTile.child("instance_effect");
                        url = nEffect.attribute("url").value();
                        std::string refName = nEffect.child("technique_hint").attribute("ref").value();

                        ColladaShader cs(dom, url);
                        cs.create(refName, entity, cs.getLibrary("library_effects"));

                        Shader& shader = entity.getComponent<Shader>();

                        ComponentTile& tc = entity.addComponent<ComponentTile>();
                        tc.tile = new Tile(new BatchRender2D(), shader, cCam.camera);
                    }
                }
            }
        }
    }
}

} // namespace Chimera