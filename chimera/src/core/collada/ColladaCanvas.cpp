#include "chimera/core/collada/ColladaCanvas.hpp"
#include "chimera/core/collada/ColladaVisualScene.hpp"

namespace Chimera {

CanvasGL* colladaCanvasGL(ColladaDom& dom) {
    CanvasGL* pCanvas = nullptr;

    pugi::xml_node vs = dom.root.child("scene");
    for (pugi::xml_node n = vs.first_child(); n; n = n.next_sibling()) {

        std::string instance = n.name();
        std::string url = n.attribute("url").value();
        if (instance == "instance_visual_scene") {

            ColladaVisualScene vs(dom, url);
            const pugi::xml_node extra = vs.getLibrary("library_visual_scenes", url).child("extra");
            if (extra != nullptr) {
                const pugi::xml_node nCanvas = getExtra(extra, "canvas");
                if (nCanvas != nullptr) {
                    std::string title = nCanvas.attribute("title").value();
                    uint32_t width = static_cast<uint32_t>(std::stoul(nCanvas.attribute("with").value()));
                    uint32_t height = static_cast<uint32_t>(std::stoul(nCanvas.attribute("height").value()));
                    bool full = false;
                    if (nCanvas.attribute("full").value() == std::string("1"))
                        full = true;

                    return new CanvasGL(title, width, height, full);
                }
            }
        }
    }

    return pCanvas;
}

} // namespace Chimera