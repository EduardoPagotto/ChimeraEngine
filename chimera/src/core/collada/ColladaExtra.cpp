#include "chimera/core/collada/ColladaExtra.hpp"
#include "chimera/core/buffer/FrameBuffer.hpp"
#include "chimera/core/collada/ColladaMaterial.hpp"
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

        std::string title = nCanvas.attribute("title").value();
        uint32_t width = static_cast<uint32_t>(std::stoul(nCanvas.attribute("width").value()));   // 1024;
        uint32_t height = static_cast<uint32_t>(std::stoul(nCanvas.attribute("height").value())); // 800;
        uint32_t f = static_cast<uint32_t>(std::stoul(nCanvas.attribute("fullscreen").value()));
        bool fullscreen = f;
        std::string type = nCanvas.attribute("type").value();

        CanvasComponent& cc = entity.addComponent<CanvasComponent>();
        if (type == "FB")
            cc.canvas = new CanvasFB(title, width, height, fullscreen);
        else
            cc.canvas = new CanvasGL(title, width, height, fullscreen);
    }

    const pugi::xml_node nEveView = getExtra(nodeExtra, "eyeview");
    if (nEveView != nullptr) {

        uint32_t size = static_cast<uint32_t>(std::stoul(nEveView.attribute("size").value()));
        float distance = std::stod(nEveView.attribute("distance").value());
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

    const pugi::xml_node nFbs = getExtra(nodeExtra, "framebuffers");
    if (nFbs != nullptr) {
        for (pugi::xml_node nFb = nFbs.first_child(); nFb; nFb = nFb.next_sibling()) {

            std::string entName = nFb.attribute("name").value();
            std::string entId = nFb.attribute("id").value();
            Entity entity = r.createEntity(entName, entId);

            FrameBufferSpecification& fb = entity.addComponent<FrameBufferSpecification>();
            for (pugi::xml_node next = nFb.first_child(); next; next = next.next_sibling()) {
                std::string name = next.name();
                if (name == "instance_material") {
                    std::string target = next.attribute("target").value();
                    // const pugi::xml_node nEffect = nFb.child("instance_effect");
                    ColladaMaterial cm(colladaDom, target);
                    cm.create(entity, next);
                }
            }
        }
    }
}
} // namespace Chimera