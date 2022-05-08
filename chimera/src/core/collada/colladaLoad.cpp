#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/collada/ColladaPhysicScene.hpp"
#include "chimera/core/collada/ColladaVisualScene.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/visible/FontManager.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

ColladaDom loadFileCollada(const std::string& file) {

    ColladaDom dom;
    dom.file = file.c_str();
    dom.pDoc = new pugi::xml_document();

    pugi::xml_parse_result result = dom.pDoc->load_file(file.c_str());
    if (result.status != pugi::status_ok) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Arquivo: %s erro: %s", file.c_str(), result.description());
        throw std::string("Falha na carga do arquivo");
    }

    SDL_Log("Load arquivo: %s Status: %s", file.c_str(), result.description());
    dom.root = dom.pDoc->child("COLLADA");

    Collada::vColladaDom.push_back(dom); // arquivo inicial

    return dom;
}

void colladaRegistryLoad(ColladaDom& dom, Registry& r) {

    const pugi::xml_node vs = dom.root.child("scene");

    // Carrega Canvas e fontes
    const pugi::xml_node extra = vs.child("extra");
    if (extra != nullptr) {
        const pugi::xml_node nCanvas = getExtra(extra, "canvas");
        if (nCanvas != nullptr) {
            std::string title = nCanvas.attribute("title").value();
            uint32_t width = static_cast<uint32_t>(std::stoul(nCanvas.attribute("with").value()));
            uint32_t height = static_cast<uint32_t>(std::stoul(nCanvas.attribute("height").value()));
            bool full = false;
            if (nCanvas.attribute("full").value() == std::string("1"))
                full = true;

            std::string entName = nCanvas.attribute("name").value();
            std::string entId = nCanvas.attribute("id").value();
            Entity entity = r.createEntity(entName, entId);
            CanvasComponent& cc = entity.addComponent<CanvasComponent>();
            cc.canvas = new CanvasGL(title, width, height, full);
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

    for (pugi::xml_node n = vs.first_child(); n; n = n.next_sibling()) {
        std::string instance = n.name();
        std::string url = n.attribute("url").value();
        if (instance == "instance_visual_scene") {

            ColladaVisualScene vs(dom, url);
            vs.loadAll(vs.getLibrary("library_visual_scenes"), &r);

        } else if (instance == "instance_physics_scene") {

            ColladaPhysicScene ps(dom, url);
            ps.loadAll(ps.getLibrary("library_physics_scenes"), &r);
        }
    }
}
} // namespace Chimera