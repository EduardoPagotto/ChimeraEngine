#include "chimera/core/collada/ColladaExtra.hpp"
#include "chimera/core/device/CanvasFB.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/visible/EyeView.hpp"
#include "chimera/core/visible/FontManager.hpp"

namespace Chimera {

void ColladaExtra::create(Registry& r, pugi::xml_node nodeExtra) {

    Entity entity;
    const pugi::xml_node nCanvas = getExtra(nodeExtra, "canvas");
    if (nCanvas != nullptr) {
        entity = r.createEntity("main_screem", "main_screem");
        std::string title = "Game";
        uint32_t width = 1024;
        uint32_t height = 800;
        bool fullscreen = true;
        setChildParam(nCanvas, "title", title);
        setChildParam(nCanvas, "width", width);
        setChildParam(nCanvas, "height", height);
        setChildParam(nCanvas, "fullscreen", fullscreen);

        std::string type = nCanvas.attribute("type").value();
        CanvasComponent& cc = entity.addComponent<CanvasComponent>();
        if (type == "FB")
            cc.canvas = new CanvasFB(title, width, height, fullscreen);
        else
            cc.canvas = new CanvasGL(title, width, height, fullscreen);
    }

    const pugi::xml_node nEveView = getExtra(nodeExtra, "eyeview");
    if (nEveView != nullptr) {

        uint32_t size = 2;
        float distance = 0.4f;
        setChildParam(nEveView, "size", size);
        setChildParam(nEveView, "distance", distance);

        EyeView& ev = entity.addComponent<EyeView>();
        if (size == 1) {
            ev.add("unique");
        } else if (size == 2) {
            ev.add("right");
            ev.add("left");
            ev.setDist(distance);
        }
    }

    const pugi::xml_node nFonts = getExtra(nodeExtra, "fonts");
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
} // namespace Chimera