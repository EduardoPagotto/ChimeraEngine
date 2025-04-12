#include "chimera/core/collada/ColladaImage.hpp"
#include "chimera/core/buffer/FrameBuffer.hpp"
#include "chimera/core/visible/TextureMng.hpp"

namespace ce {

static void setRange(const std::string& range, TexDType& type) {
    if (range == "FLOAT")
        type = TexDType::FLOAT;
    else if (range == "UINT")
        type = TexDType::UNSIGNED_BYTE;
    else if (range == "SINT")
        type = TexDType::UNSIGNED_SHORT; // TODO: ver todos os outros tipos!!!!!
}

static void setChannelTexFormat(const std::string& channel, TexFormat& format) {
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
    if (entity.hasComponent<FrameBufferSpecification>() == true) {
        FrameBufferSpecification& frames = entity.getComponent<FrameBufferSpecification>();
        fb = &frames;
    }

    for (pugi::xml_node nImg = node.first_child(); nImg; nImg = nImg.next_sibling()) {
        std::string field = nImg.name();
        if (field == "create_2d") {

            uint32_t width = 128, height = 128;
            pugi::xml_node nSize = nImg.child("size_exact");
            if (nSize != nullptr) {
                width = static_cast<uint32_t>(std::stoul(nSize.attribute("width").value()));
                height = static_cast<uint32_t>(std::stoul(nSize.attribute("height").value()));
            }

            if (pugi::xml_node nFormat = nImg.child("format"); nFormat != nullptr) {
                if (pugi::xml_node nHint = nFormat.child("hint"); nHint != nullptr) {
                    setChannelTexFormat(nHint.attribute("channels").value(), tp.format);
                    setChannelTexFormat(nHint.attribute("channelsInternal").value(), tp.internalFormat);
                    setRange(nHint.attribute("range").value(), tp.type);
                }
            }

            if (fb != nullptr) {
                if (nSize != nullptr) {
                    fb->width = width;
                    fb->height = height;
                }
                fb->attachments.push_back(tp);
            }

        } else if (field == "init_from") {
            if (pugi::xml_text pathFile = nImg.text(); pathFile != nullptr) {
                std::string f = pathFile.as_string();
                SDL_Log("Nova textura %s, Key: %s", f.c_str(), id.c_str());
                auto texMng = g_service_locator.getService<TextureMng>();
                texMng->loadFromFile(id, f, tp);
                return;
            }
            throw std::string("Textura nao encontrada: " + id);
        }
    }
}
} // namespace ce