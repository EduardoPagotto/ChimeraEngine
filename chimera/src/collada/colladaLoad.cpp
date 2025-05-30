#include "chimera/collada/colladaLoad.hpp"
#include "chimera/collada/ColladaExtra.hpp"
#include "chimera/collada/ColladaPhysicScene.hpp"
#include "chimera/collada/ColladaVisualScene.hpp"
#include <SDL3/SDL.h>

namespace ce {

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

    void colladaRegistryLoad(ColladaDom& dom) {

        const pugi::xml_node vs = dom.root.child("scene");
        for (pugi::xml_node n = vs.first_child(); n; n = n.next_sibling()) {
            std::string instance = n.name();
            std::string url = n.attribute("url").value();

            if (instance == "extra") {

                ColladaExtra ce(dom, "#vazio");
                ce.create(n);

            } else if (instance == "instance_visual_scene") {

                ColladaVisualScene vs(dom, url);
                vs.loadAll(vs.getLibrary("library_visual_scenes"));

            } else if (instance == "instance_physics_scene") {

                ColladaPhysicScene ps(dom, url);
                ps.loadAll(ps.getLibrary("library_physics_scenes"));
            }
        }
    }
} // namespace ce
