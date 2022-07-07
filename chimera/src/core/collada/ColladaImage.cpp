#include "chimera/core/collada/ColladaImage.hpp"
#include "chimera/core/buffer/FrameBuffer.hpp"
#include "chimera/core/visible/TextureManager.hpp"

namespace Chimera {

static TexFilter setFilter(const std::string& sParamVal) {
    if (sParamVal == "NEAREST")
        return TexFilter::NEAREST;
    else if (sParamVal == "LINEAR")
        return TexFilter::LINEAR;

    return TexFilter::NONE;
}

static TexWrap setWrap(const std::string& sParamVal) {
    if (sParamVal == "WRAP")
        return TexWrap::REPEAT;
    else if (sParamVal == "MIRROR")
        return TexWrap::MIRRORED;
    else if (sParamVal == "CLAMP")
        return TexWrap::CLAMP_TO_EDGE;
    else if (sParamVal == "BORDER")
        return TexWrap::CLAMP_TO_BORDER;

    return TexWrap::NONE;
}

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

ColladaImage::~ColladaImage() {
    mapaTex.clear();
    mapa2D.clear();
}

void ColladaImage::loadImage(const std::string& id, TexParam& tp) {

    FrameBufferSpecification* fb = nullptr;
    if (entity.hasComponent<FrameBufferSpecification>() == true) {
        FrameBufferSpecification& frames = entity.getComponent<FrameBufferSpecification>();
        fb = &frames;
    }

    pugi::xml_node node = getLibraryUrl("library_images", id);
    for (pugi::xml_node nImg = node.first_child(); nImg; nImg = nImg.next_sibling()) {
        std::string field = nImg.name();
        if (field == "create_2d") {

            uint32_t width = 128, height = 128;
            pugi::xml_node nSize = nImg.child("size_exact");
            if (nSize != nullptr) {
                width = static_cast<uint32_t>(std::stoul(nSize.attribute("width").value()));
                height = static_cast<uint32_t>(std::stoul(nSize.attribute("height").value()));
            }

            pugi::xml_node nFormat = nImg.child("format");
            if (nFormat != nullptr) {
                pugi::xml_node nHint = nFormat.child("hint");
                if (nHint != nullptr) {
                    setChannelTexFormat(nHint.attribute("channels").value(), tp.format);
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
            pugi::xml_text pathFile = nImg.text();
            if (pathFile != nullptr) {
                std::string f = pathFile.as_string();
                SDL_Log("Nova textura %s, Key: %s", f.c_str(), id.c_str());
                TextureManager::loadFromFile(id, f, tp);
                return;
            }
            throw std::string("Textura nao encontrada: " + id);
        }
    }
}

bool ColladaImage::setTextureParam(const pugi::xml_node& n, TexParam& tp) {
    for (pugi::xml_node ntPara = n.first_child(); ntPara; ntPara = ntPara.next_sibling()) {
        std::string sParam = ntPara.name();
        std::string sParamVal = ntPara.text().as_string();
        if (sParam == "minfilter")
            tp.minFilter = setFilter(sParamVal);
        else if (sParam == "magfilter")
            tp.magFilter = setFilter(sParamVal);
        else if (sParam == "wrap_s")
            tp.wrap_s = setWrap(sParamVal);
        else if (sParam == "wrap_t")
            tp.wrap_t = setWrap(sParamVal);
        else if (sParam == "wrap_r")
            tp.wrap_r = setWrap(sParamVal);
        else if (sParam == "instance_image") {

            std::string url = ntPara.attribute("url").value();
            loadImage(url, tp);
            return true;
        }
    }
    return false;
}

void ColladaImage::create(Entity entity, pugi::xml_node nodeParent) {

    this->entity = entity;
    for (pugi::xml_node param = nodeParent.first_child(); param; param = param.next_sibling()) {

        TexParam tp;
        std::string sProf = param.name();
        std::string sid = param.attribute("sid").value();
        if (sProf == "newparam") {

            pugi::xml_node val1 = param.first_child();
            std::string sVal1 = val1.name();

            if (sVal1 == "surface") {

                std::string keyImage = val1.child("init_from").text().as_string();
                loadImage(keyImage, tp);
                mapaTex[sid] = keyImage;

            } else if (sVal1 == "sampler2D") {
                if (setTextureParam(val1, tp) == false) {
                    std::string keyMap = val1.child("source").text().as_string();
                    mapa2D[sid] = keyMap;
                }
            } else if (sVal1 == "samplerDEPTH") {
                // nao e textura e FR
                tp.format = TexFormat::DEPTH_COMPONENT;
                tp.internalFormat = TexFormat::DEPTH_COMPONENT;
                setTextureParam(val1, tp);
            }
        }
    }
}

Texture* ColladaImage::getTexureByName(const std::string& name) {
    std::string idTex = mapaTex[mapa2D[name]];
    return TextureManager::get(idTex);
}
} // namespace Chimera