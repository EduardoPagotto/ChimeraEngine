#include "chimera/core/collada/ColladaCanvas.hpp"
#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/collada/ColladaVisualScene.hpp"
#include "chimera/core/visible/FontManager.hpp"

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
            }
        }
    }

    return pCanvas;
}

} // namespace Chimera