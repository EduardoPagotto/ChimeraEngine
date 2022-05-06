#include "chimera/core/collada/ColladaCanvas.hpp"
#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/collada/ColladaShader.hpp"
#include "chimera/core/collada/ColladaVisualScene.hpp"
#include "chimera/core/visible/Components.hpp"
#include "chimera/core/visible/FontManager.hpp"
#include "chimera/core/visible/Shader.hpp"

//#include "chimera/render/2d/BatchRender2D.hpp"
//#include "chimera/render/scene/Components.hpp"

namespace Chimera {

CanvasGL* colladaCanvasGL(ColladaDom& dom, Registry& r) {
    CanvasGL* pCanvas = nullptr;

    pugi::xml_node vs = dom.root.child("scene");
    for (pugi::xml_node n = vs.first_child(); n; n = n.next_sibling()) {

        std::string instance = n.name();
        std::string url = n.attribute("url").value();
        if (instance == "instance_visual_scene") {

            ColladaVisualScene vs(dom, url);
            const pugi::xml_node extra = vs.getLibrary("library_visual_scenes").child("extra");
            if (extra != nullptr) {
                const pugi::xml_node nCanvas = getExtra(extra, "canvas");
                if (nCanvas != nullptr) {
                    std::string title = nCanvas.attribute("title").value();
                    uint32_t width = static_cast<uint32_t>(std::stoul(nCanvas.attribute("with").value()));
                    uint32_t height = static_cast<uint32_t>(std::stoul(nCanvas.attribute("height").value()));
                    bool full = false;
                    if (nCanvas.attribute("full").value() == std::string("1"))
                        full = true;

                    pCanvas = new CanvasGL(title, width, height, full);
                }

                const pugi::xml_node nFonts = getExtra(extra, "fonts");
                if (nFonts != nullptr) {
                    for (pugi::xml_node nFont = nFonts.first_child(); nFont; nFont = nFont.next_sibling()) {

                        RFC3986 rfc(nFont.attribute("url").value());
                        int size = static_cast<int>(std::stoul(nFont.attribute("size").value()));
                        float scaleX = std::stod(nFont.attribute("scaleX").value());
                        float scaleY = std::stod(nFont.attribute("scaleY").value());
                        FontManager::add(new FontAtlas(rfc.getFragment(), rfc.getPath(), size));
                        FontManager::get()->setScale(glm::vec2(scaleX, scaleY));
                    }
                }

                // const pugi::xml_node nTiles = getExtra(extra, "tiles");
                // if (nTiles != nullptr) {
                //     for (pugi::xml_node nTile = nTiles.first_child(); nTile; nTile = nTile.next_sibling()) {

                //         Entity entity = r.createEntity(nTile.attribute("name").value(), nTile.attribute("id").value());
                //         const pugi::xml_node nCam = nTile.child("instance_camera");
                //         std::string url = nCam.attribute("url").value();

                //         ColladaCam cc(dom, url);
                //         cc.create(entity, cc.getLibrary("library_cameras"));

                //         ComponentCamera& cCam = entity.getComponent<ComponentCamera>();

                //         const pugi::xml_node nEffect = nTile.child("instance_effect");
                //         std::string url2 = nEffect.attribute("url").value();
                //         std::string refName = nEffect.child("technique_hint").attribute("ref").value();

                //         ColladaShader cs(dom, url2);
                //         cs.create(refName, entity, cs.getLibrary("library_effects"));

                //         Shader& shader = entity.getComponent<Shader>();

                //         // ComponentTile& tc = entity.addComponent<ComponentTile>();
                //         // tc.tile = new Tile(new BatchRender2D(), shader, cCam.camera);
                //     }
                // }
            }
        }
    }

    return pCanvas;
}

} // namespace Chimera