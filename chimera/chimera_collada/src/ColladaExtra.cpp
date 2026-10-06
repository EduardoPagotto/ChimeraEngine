#include "chimera_collada/ColladaExtra.hpp"
#include "chimera_collada/ColladaCam.hpp"
#include "chimera_collada/ColladaEffect.hpp"
#include "chimera_collada/RFC3986.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include "chimera_core/gl/buffer/FrameBuffer.hpp"

namespace ce {

    void ColladaExtra::create(pugi::xml_node node_extra) {

        if (const pugi::xml_node n_fonts = getExtra(node_extra, "fonts"); n_fonts != nullptr) {
            for (pugi::xml_node n_font = n_fonts.first_child(); n_font; n_font = n_font.next_sibling()) {

                RFC3986 rfc(n_font.attribute("url").value());
                int size = static_cast<int>(std::stoul(n_font.attribute("size").value()));
                float scale_x = std::stod(n_font.attribute("scaleX").value());
                float scale_y = std::stod(n_font.attribute("scaleY").value());

                auto asset = registry->ctx().get<std::shared_ptr<ce::AssetManager>>();

                auto font = asset->load_font(rfc.get_fragment(), rfc.get_path(), size);
                font->scale = glm::vec2(scale_x, scale_y);
            }
        }

        if (const pugi::xml_node n_fbs = getExtra(node_extra, "framebuffers"); n_fbs != nullptr) {
            for (pugi::xml_node n_fb = n_fbs.first_child(); n_fb; n_fb = n_fb.next_sibling()) {

                std::string ent_name = n_fb.attribute("name").value();
                std::string ent_id = n_fb.attribute("id").value();
                Entity entity = Entity::create(registry.get(), ent_name, ent_id);

                [[maybe_unused]]
                FrameBufferSpecification& fb = entity.add_component<FrameBufferSpecification>(registry.get());
                for (pugi::xml_node next = n_fb.first_child(); next; next = next.next_sibling()) {
                    std::string name = next.name();
                    std::string url = next.attribute("url").value();
                    if (name == "instance_effect") {

                        std::string ref_name = next.child("technique_hint").attribute("ref").value();
                        ColladaEffect cf(registry, colladaDom, url);
                        cf.create(ref_name, entity, cf.get_library("library_effects"));

                    } else if (name == "instance_camera") {

                        ColladaCam cc(registry, colladaDom, url);
                        cc.create(entity, cc.get_library("library_cameras"));
                        cc.create_extra(entity, next.first_child());
                    }
                }
            }
        }
    }
} // namespace ce
