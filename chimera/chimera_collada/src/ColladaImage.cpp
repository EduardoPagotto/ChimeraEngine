#include "chimera_collada/ColladaImage.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include "chimera_core/gl/buffer/FrameBuffer.hpp"

namespace ce {

    static void set_range(const std::string& range, TexDType& type) {
        if (range == "FLOAT")
            type = TexDType::FLOAT;
        else if (range == "UINT")
            type = TexDType::UNSIGNED_BYTE;
        else if (range == "SINT")
            type = TexDType::UNSIGNED_SHORT; // TODO: ver todos os outros tipos!!!!!
    }

    static void set_channel_tex_format(const std::string& channel, TexFormat& format) {
        if (channel == "RGB")
            format = TexFormat::RGB;
        else if (channel == "RGBA")
            format = TexFormat::RGBA;
        else if (channel == "RGB8")
            format = TexFormat::RGBA8; // chimera only
        else if (channel == "L")
            format = TexFormat::LUMINANCE;
        else if (channel == "LA")
            format = TexFormat::LUMINANCE_ALPHA;
        else if (channel == "D")
            format = TexFormat::DEPTH_COMPONENT;
        else if (channel == "DA")
            format = TexFormat::DEPTH_ATTACHMENT; // chimera only
        else if (channel == "D24")
            format = TexFormat::DEPTH24STENCIL8; // chimera only
        else if (channel == "R32I")
            format = TexFormat::R32I; // chimera only
        else if (channel == "REDI")
            format = TexFormat::RED_INTEGER; // chimera only
    }

    void ColladaImage::create(Entity entity, TexParam& tp, const pugi::xml_node& node) {

        std::string id = node.attribute("id").value();

        FrameBufferSpecification* fb = nullptr;
        if (entity.has_component<FrameBufferSpecification>(registry.get()) == true) {
            FrameBufferSpecification& frames = entity.get_component<FrameBufferSpecification>(registry.get());
            fb = &frames;
        }

        for (pugi::xml_node n_img = node.first_child(); n_img; n_img = n_img.next_sibling()) {
            std::string field = n_img.name();
            if (field == "create_2d") {

                uint32_t width = 128, height = 128;
                pugi::xml_node n_size = n_img.child("size_exact");
                if (n_size != nullptr) {
                    width = static_cast<uint32_t>(std::stoul(n_size.attribute("width").value()));
                    height = static_cast<uint32_t>(std::stoul(n_size.attribute("height").value()));
                }

                if (pugi::xml_node n_format = n_img.child("format"); n_format != nullptr) {
                    if (pugi::xml_node n_hint = n_format.child("hint"); n_hint != nullptr) {
                        set_channel_tex_format(n_hint.attribute("channels").value(), tp.format);
                        set_channel_tex_format(n_hint.attribute("channelsInternal").value(), tp.internalFormat);
                        set_range(n_hint.attribute("range").value(), tp.type);
                    }
                }

                if (fb != nullptr) {
                    if (n_size != nullptr) {
                        fb->width = width;
                        fb->height = height;
                    }
                    fb->attachments.push_back(tp);
                }

            } else if (field == "init_from") {
                if (pugi::xml_text path_file = n_img.text(); path_file != nullptr) {
                    std::string f = path_file.as_string();
                    SDL_Log("Nova textura %s, Key: %s", f.c_str(), id.c_str());

                    auto assets = this->registry->ctx().get<std::shared_ptr<AssetManager>>();

                    assets->load_texture(id, f, tp);
                    return;
                }
                throw std::string("Textura nao encontrada: " + id);
            }
        }
    }
} // namespace ce
