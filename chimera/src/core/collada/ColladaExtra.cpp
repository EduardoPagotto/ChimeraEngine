#include "chimera/core/collada/ColladaExtra.hpp"
#include "chimera/core/ViewProjection.hpp"
#include "chimera/core/buffer/FrameBuffer.hpp"
#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/collada/ColladaEffect.hpp"
#include "chimera/core/device/CanvasFB.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/visible/FontManager.hpp"

namespace Chimera {

void ColladaExtra::create(pugi::xml_node nodeExtra) {

    if (const pugi::xml_node nFonts = getExtra(nodeExtra, "fonts"); nFonts != nullptr) {
        for (pugi::xml_node nFont = nFonts.first_child(); nFont; nFont = nFont.next_sibling()) {

            RFC3986 rfc(nFont.attribute("url").value());
            int size = static_cast<int>(std::stoul(nFont.attribute("size").value()));
            float scaleX = std::stod(nFont.attribute("scaleX").value());
            float scaleY = std::stod(nFont.attribute("scaleY").value());
            FontManager::add(new FontAtlas(rfc.getFragment(), rfc.getPath(), size));
            FontManager::get()->setScale(glm::vec2(scaleX, scaleY));
        }
    }

    if (const pugi::xml_node nFbs = getExtra(nodeExtra, "framebuffers"); nFbs != nullptr) {
        for (pugi::xml_node nFb = nFbs.first_child(); nFb; nFb = nFb.next_sibling()) {

            std::string entName = nFb.attribute("name").value();
            std::string entId = nFb.attribute("id").value();
            auto r = serviceLoc->getService<Registry>();
            Entity entity = r->createEntity(entName, entId);

            FrameBufferSpecification& fb = entity.addComponent<FrameBufferSpecification>();
            for (pugi::xml_node next = nFb.first_child(); next; next = next.next_sibling()) {
                std::string name = next.name();
                std::string url = next.attribute("url").value();
                if (name == "instance_effect") {

                    std::string refName = next.child("technique_hint").attribute("ref").value();
                    ColladaEffect cf(colladaDom, url, serviceLoc);
                    cf.create(refName, entity, cf.getLibrary("library_effects"));

                } else if (name == "instance_camera") {

                    ColladaCam cc(colladaDom, url, serviceLoc);
                    cc.create(entity, cc.getLibrary("library_cameras"));
                    cc.createExtra(entity, next.first_child());
                }
            }
        }
    }
}
} // namespace Chimera