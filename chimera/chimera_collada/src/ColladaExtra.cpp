#include "chimera_collada/ColladaExtra.hpp"
#include "chimera_collada/ColladaCam.hpp"
#include "chimera_collada/ColladaEffect.hpp"
#include "chimera_collada/RFC3986.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include "chimera_core/gl/buffer/FrameBuffer.hpp"

namespace ce {

    void ColladaExtra::create(pugi::xml_node nodeExtra) {

        if (const pugi::xml_node nFonts = getExtra(nodeExtra, "fonts"); nFonts != nullptr) {
            for (pugi::xml_node nFont = nFonts.first_child(); nFont; nFont = nFont.next_sibling()) {

                RFC3986 rfc(nFont.attribute("url").value());
                int size = static_cast<int>(std::stoul(nFont.attribute("size").value()));
                float scaleX = std::stod(nFont.attribute("scaleX").value());
                float scaleY = std::stod(nFont.attribute("scaleY").value());

                auto asset = registry->ctx().get<std::shared_ptr<ce::AssetManager>>();

                auto font = asset->loadFont(rfc.getFragment(), rfc.getPath(), size);
                font->scale = glm::vec2(scaleX, scaleY);
            }
        }

        if (const pugi::xml_node nFbs = getExtra(nodeExtra, "framebuffers"); nFbs != nullptr) {
            for (pugi::xml_node nFb = nFbs.first_child(); nFb; nFb = nFb.next_sibling()) {

                std::string entName = nFb.attribute("name").value();
                std::string entId = nFb.attribute("id").value();
                Entity entity(entName, entId);

                [[maybe_unused]]
                FrameBufferSpecification& fb = entity.addComponent<FrameBufferSpecification>();
                for (pugi::xml_node next = nFb.first_child(); next; next = next.next_sibling()) {
                    std::string name = next.name();
                    std::string url = next.attribute("url").value();
                    if (name == "instance_effect") {

                        std::string refName = next.child("technique_hint").attribute("ref").value();
                        ColladaEffect cf(registry, colladaDom, url);
                        cf.create(refName, entity, cf.getLibrary("library_effects"));

                    } else if (name == "instance_camera") {

                        ColladaCam cc(registry, colladaDom, url);
                        cc.create(entity, cc.getLibrary("library_cameras"));
                        cc.createExtra(entity, next.first_child());
                    }
                }
            }
        }
    }
} // namespace ce
