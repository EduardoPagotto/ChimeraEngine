#include "chimera_collada/Collada.hpp"
#include "chimera_base/aux/utils.hpp"
#include "chimera_collada/RFC3986.hpp"
#include <SDL3/SDL.h>
#include <glm/gtc/type_ptr.hpp>

namespace ce {

    const glm::vec4 textToVec4(const std::string& text) {
        std::vector<float> array_float;
        text_to_float_array(text, array_float);
        if (array_float.size() == 4)
            return glm::vec4(array_float[0], array_float[1], array_float[2], array_float[3]);

        return glm::vec4(array_float[0], array_float[1], array_float[2], 1.0f);
    }

    const glm::vec3 textToVec3(const std::string& text) {
        std::vector<float> array_float;
        text_to_float_array(text, array_float);

        return glm::vec3(array_float[0], array_float[1], array_float[2]);
    }

    const glm::mat4 textToMat4(const std::string& text) {

        std::vector<float> array_float;
        text_to_float_array(text, array_float);

        if (array_float.size() != 16)
            throw std::string("Tamanho da Matrix invalido" + std::to_string(array_float.size()));

        float ponteiro_float[16];
        int indice = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int pos = i + (4 * j);
                ponteiro_float[pos] = array_float[indice];
                indice++;
            }
        }

        return glm::make_mat4(&ponteiro_float[0]);
    }

    //--

    Collada::Collada(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
        : registry(registry) {

        RFC3986 rfc(url);
        if (rfc.is_invalid() == true)
            throw std::string("URL " + url + " invalida");

        if (rfc.get_scheme() == RFC3986_SCHEME::LOCAL)
            colladaDom = dom;
        else {

            for (auto dom_cache : Collada::v_collada_dom) {
                if (dom_cache.file == rfc.get_path()) {
                    colladaDom = dom_cache;
                    fragment_ = rfc.get_fragment();
                    SDL_Log("Arquivo %s cache, id: %s", colladaDom.file.c_str(), rfc.get_fragment().c_str());
                    return;
                }
            }

            colladaDom.file = rfc.get_path();
            colladaDom.pDoc = new pugi::xml_document();
            pugi::xml_parse_result result = colladaDom.pDoc->load_file(colladaDom.file.c_str());
            if (result.status != pugi::status_ok)
                throw std::string("Arquivo " + colladaDom.file + " erro: %s" + std::string(result.description()));

            SDL_Log("Arquivo %s novo, id: %s Status: %s", colladaDom.file.c_str(), rfc.get_fragment().c_str(),
                    result.description());
            colladaDom.root = colladaDom.pDoc->child("COLLADA");

            Collada::v_collada_dom.push_back(colladaDom);
        }

        fragment_ = rfc.get_fragment();
    }

    void Collada::destroy() {

        for (ColladaDom dom : v_collada_dom)
            dom.pDoc->reset();

        while (v_collada_dom.size() != 0) {
            std::vector<ColladaDom>::iterator it = v_collada_dom.begin();
            delete (*it).pDoc;
            (*it).pDoc = nullptr;

            v_collada_dom.erase(it);
        }
    }

    const pugi::xml_node Collada::get_library(const std::string& library_name) {
        return get_library_key(library_name, fragment_);
    }

    const pugi::xml_node Collada::get_library_key(const std::string& library_name, const std::string& key) {
        for (pugi::xml_node n = colladaDom.root.first_child(); n; n = n.next_sibling()) {
            std::string name = n.name();
            if (name == library_name) {

                for (pugi::xml_node t = n.first_child(); t; t = t.next_sibling()) {
                    if (std::string id = t.attribute("id").value(); id == key) {
                        SDL_Log("%s: %s id: %s", library_name.c_str(), t.name(), id.c_str());
                        return t;
                    }
                }
            }
        }

        throw std::string(library_name + " não encontrado id: " + key);
    }

    const pugi::xml_node Collada::get_library_url(const std::string& library_name, const std::string& url) {
        std::size_t found = url.find('#');
        std::string key = (found != std::string::npos) ? url.substr(found + 1, std::string::npos) : url;
        return get_library_key(library_name, key);
    }

    const pugi::xml_node getExtra(const pugi::xml_node node, const std::string& name) {

        for (pugi::xml_node n_tec = node.first_child(); n_tec; n_tec = n_tec.next_sibling()) {
            if ((strcmp(n_tec.name(), "technique") == 0) and
                (strcmp(n_tec.attribute("profile").value(), "chimera") == 0))
                return n_tec.child(name.c_str());
        }

        return pugi::xml_node();
    }

} // namespace ce
